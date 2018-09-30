#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <deque>



namespace culminate {

  namespace literal {
  
    /**
     * Support for free style text in the middle of the column formatting
     **/
    struct FreeStyle
    {
      FreeStyle(char const * const str): _str(str) {}
      char const * const _str;
    };
    
    inline FreeStyle operator"" _free (char const * const str, unsigned long )
    {
      return FreeStyle(str);
    }
  }

  namespace decorator
  {
    using Tool = std::function<std::ostream&(std::ostream&, const std::string&)>;

    inline std::ostream& left(std::ostream& os, const std::string&) { return os << std::left; } 
    inline std::ostream& right(std::ostream& os, const std::string&) { return os << std::right;}
    inline std::ostream& center(std::ostream& os, const std::string& str)
    {
      std::string& updateStr = const_cast<std::string&>(str);
      std::streamsize width = os.width();
      if ( width > str.size())
      {
        size_t sz = str.size();
        std::streamsize left = width - ((width + str.size()) / 2 );
        os.width(left);
        os << "";
        os.width(width - left );
        updateStr += std::string(width - left - sz, ' ');
      }
      return os;
    }

    template <typename T>
    inline Tool code(const T& c)
    {
      return [c](std::ostream& os, const std::string&)  -> std::ostream&
      {
        size_t sz(os.width());
        os.width(0);
        os << c;
        os.width(sz);
        return os;
      };
    }
  }
  
  using manipulator = std::ostream&(*)(std::ostream&);
  using justification = std::ios_base& (*)(std::ios_base&);

  static inline std::string& ltrim(std::string &s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }

  // trim from end
  static inline std::string& rtrim(std::string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  // trim from both ends
  static inline std::string& trim(std::string &s)
  {
    return ltrim(rtrim(s));
  }

  /**
   * Used as configuration class for an entire idnetiation level
   * Including but not restircted to max column max size.
   **/
  class Level
  {
    public:

      static std::ostream& next(std::ostream& stream) { return stream; }
      static std::ostream& prev(std::ostream& stream) { return stream; }

      Level(size_t indent = 0, const std::string& sep = "  ")
      : _indent(indent * 2, ' '), _sep(sep) {}

      const std::string& separator() const { return _sep; }
      const std::string& indent() const { return _indent; }
      const size_t size() const { return _col.size(); }

      size_t& columnSize(size_t index, size_t newSize)
      {
        return _col[index]._width = std::max(newSize, _col[index]._width);
      }

      void updateWidths(const std::vector<std::string>& values)
      {
        for(size_t i = 0, iEnd = values.size(); i < iEnd; ++i)
        {
          columnSize(i, values[i].size());
        }
      }

      void indent(size_t indent) { _indent = std::string(indent, ' '); } 

      // A Cells Configuration
      struct Configuration
      {
        enum class Type { Default, Numeric, Alpha };
        Configuration(): _width(0), _type(Type::Default) {}

        size_t                        _width;       // Coloumn width
        Type                          _type;
        std::vector<decorator::Tool>  _decorators;  // Extra decorating attributes

        decorator::Tool justify()
        {
          return _type == Type::Numeric ? decorator::right : decorator::left;
        }

        void setNumeric(bool numeric ) 
        {
           _type = not numeric ? Type::Alpha
                               : _type == Type::Default or _type == Type::Numeric ? Type::Numeric : Type::Alpha;
        }

        Configuration& apply(decorator::Tool decor) { _decorators.emplace_back(decor); return *this;}

        void apply(std::ostream& stream, const std::string& str) const
        {
          for(auto decorator : _decorators)
          {
            decorator(stream, str);
          }
        }
      };

      Configuration& column(size_t index) { return _col[index]; }

    private:

      std::string                     _sep;     // Separator
      std::string                     _indent;  // Indentation
      std::map<size_t, Configuration> _col;     // Index -> size
  };


  /**
   * One field of information abstraction
   */
  class Cell
  {
    public:
      using ConfFunc = std::function<Level::Configuration&()>;

      Cell(const std::string& value, ConfFunc getConf)
      : _value(value), _config(getConf) 
      {
        _config()._width = std::max(_config()._width, value.size());
      }

      const std::string& value() const { return _value; }

      size_t size() const {  return _config()._width; }
      std::ostream& justify(std::ostream& os) const { return _config().justify()(os, _value); }

      void config(std::ostream& os) const
      {
        os << std::setw(size());
        justify(os); 
        _config().apply(os, _value);
      }

      void isNumeric(bool numeric) { _config().setNumeric(numeric); }

    private:
      std::string  _value;
      ConfFunc     _config;
  };

  /**
   * A Row contains 1-n Cells.
   **/
  class Row
  {
    public:
      Row(Level& level): _level(level) {}

      template <typename T ,
                typename std::enable_if<!std::is_integral<T>::value>::type* = nullptr>
      void add(const T& value) { add(std::to_string(value)); }

      template <typename T,
                typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
      void add(const T& value) 
      {
        Cell& added = add(std::to_string(value), true /* Numeric */); 
      }

      void add(const char* value) { add(std::string(value)); }
      Cell& add(const std::string& value, bool numeric = false)
      {
        std::string trimmed(value);
        trim(trimmed);

        // TODO: Binding on this relies on this staying at the same position. While a vector growth may invalidate it
        _cell.emplace_back(trimmed, [this, idx=_cell.size()]() -> Level::Configuration& { return _level.column(idx); });
        Cell& cell = _cell.back();
        cell.isNumeric(numeric);

        return cell;
      }

      void add(const literal::FreeStyle& f)
      {
        _cell.emplace_back(f._str, []() -> Level::Configuration& { 
          static Level::Configuration conf;
          return conf;});
      }

      // TODO: Handle rvalue references, for increase efficiency.

      void display(std::ostream& stream) const
      {
        stream << _level.indent();
        for(auto& cell : _cell)
        {
          cell.config(stream);
          stream  << cell.value() << _level.separator();
        }
        stream << "\n";
      }

    private:
      std::vector<Cell> _cell;
      Level&            _level;
  };

  /**
   * Heavy duty stream
   */
  class Surge
  {
    
    public:
      Surge(const std::vector<std::string>& names = {}, std::ostream& out = std::cout)
      : _out(out), _current(_level.end())
      {
        title(names);
      }


      ~Surge() { flush(); }

      void flush()
      {
        // Support Titles only for first Level.
        if (_current != _level.end() and _title) { _title(_out, *_current); } 
        for(auto& Row : _row) { Row.display(_out); }
        _row.clear();
      }

      template <typename T>
      Surge& operator<<(const T& value)
      {
        row().add(value);
        return *this;
      }

      void replaceRow()
      {
        _row.pop_back();
        _row.emplace_back(level());
      }

      Surge& operator<<(manipulator m)
      {
        if (m == static_cast<manipulator>(std::endl))       { _row.emplace_back(level()); }
        else if (m == static_cast<manipulator>(std::ends))  { flush(); }
        else if (m == static_cast<manipulator>(Level::next))  { nextLevel(); replaceRow(); } 
        else if (m == static_cast<manipulator>(Level::prev))  { prevLevel(); replaceRow(); }
        return *this;
      }

      Row& row()
      {
        if (_row.empty()) { _row.emplace_back(level()); }
        return _row.back();
      }

      Level& level(size_t level)
      {
        auto itr = _level.begin();
        for (size_t i = _level.size(); i <= level; ++i)
        {
          addLevel();
        }
        return *_levelCache[level];
      }

      Level& level()
      {
        if ( _current == _level.end()) 
        { 
          _current = addLevel();
        }  
        return *_current;
      }

      void title(const std::vector<std::string>& names)
      {
        _title = [names](std::ostream& stream, Level& level) {
          stream << level.indent();
          for(size_t i = 0, iEnd = names.size(); i < iEnd; ++i)
          {
            size_t size = i < level.size() ? level.column(i)._width : names[i].size();
            stream << std::setw(size);
            decorator::center(stream, names[i]);
            stream  << names[i].substr(0, size) << level.separator();
          }
          stream << "\n";
          for(size_t i = 0, iEnd = names.size(); i < iEnd; ++i)
          {
            size_t size = i < level.size() ? level.column(i)._width : names[i].size();
            stream  << std::setfill('-') << std::setw(size) << "" << level.separator();
          }
          stream << std::setfill(' ') << "\n";
        };
        level().updateWidths(names);
      }

    private:
      using Levels   = std::list<Level>;
      using LevelItr = Levels::iterator;

      std::deque<Row>  _row;
      Levels            _level;
      LevelItr          _current;
      std::unordered_map<size_t, LevelItr> _levelCache;

      std::ostream&     _out;
      std::function<void(std::ostream&, Level&)> _title = nullptr;

      LevelItr  addLevel()
      {
          size_t index( _level.size() );
          _level.emplace_back( index ); 
          return _levelCache[ index ] = std::prev(_level.end());
      }

      LevelItr nextLevel()
      {
        if (std::next(_current) == _level.end())
        {
          addLevel();
        }
        return ++_current;
      }

      LevelItr prevLevel()
      {
        return --_current;
      }
  };
};
