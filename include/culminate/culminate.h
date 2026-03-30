#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <array>
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
    inline std::ostream& right(std::ostream& os, const std::string&){ return os << std::right;}
    inline std::ostream& center(std::ostream& os, const std::string&)
    {
      // Note: Use Configuration.center() method instead of this decorator
      // This decorator alone does not set center alignment
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

    template <typename T>
    inline Tool conditionalCode(std::function<bool(const std::string&)> f, const T& c)
    {
      return [f, c](std::ostream& os, const std::string& str) -> std::ostream&
      {
        if (f(str))
        {
          size_t sz(os.width());
          os.width(0);
          os << c;
          os.width(sz);
        }
        return os;
      };
    }


    template <typename T>
    inline Tool conditionalCode(std::function<bool(const std::string&)> f, const T& c1, const T& c2)
    {
      return [f, c1, c2](std::ostream& os, const std::string& str) -> std::ostream&
      {
        size_t sz(os.width());
        os.width(0);
        os << (f(str) ? c1 : c2);
        os.width(sz);
        return os;
      };
    }

    template <typename T>
    inline Tool conditionalCode(std::function<bool(int)> f, const T& c1, const T& c2)
    {
      return [f, c1, c2](std::ostream& os, const std::string& str) -> std::ostream&
      {
        size_t sz(os.width());
        os.width(0);
        try {
          os << (f(std::stoi(str)) ? c1 : c2);
        } catch (...) {
          os << c2;
        }
        os.width(sz);
        return os;
      };
    }

  }
  
  using manipulator = std::ostream&(*)(std::ostream&);
  using justification = std::ios_base& (*)(std::ios_base&);

  static inline bool isSpace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
  }

  static inline std::string& ltrim(std::string &s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
          [](unsigned char c) { return !std::isspace(c); }));
    return s;
  }

  // trim from end
  static inline std::string& rtrim(std::string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(),
          [](unsigned char c) { return !std::isspace(c); }).base(), s.end());
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
      : _sep(sep), _indent(indent * 2, ' ') {}

      const std::string& separator() const { return _sep; }
      const std::string& indent() const { return _indent; }
      size_t size() const { return _col.size(); }

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

      Level& indent(size_t indent) { _indent = std::string(indent, ' '); return *this; } 

      // A Cells Configuration
      struct Configuration
      {
        static constexpr size_t NumOrders = 2;
        enum class Order { Pre, Post };
        enum class Type { Default, Numeric, Alpha, Center };
        Configuration(): _width(0), _type(Type::Default) {}

        bool    _visible = true;
        size_t  _width;       // Coloumn width
        Type    _type;
        std::array<std::vector<decorator::Tool>, NumOrders>  _decorators;

        constexpr bool visible() const { return _visible; }
        Configuration& hide() { _visible = false; return *this; }
        decorator::Tool justify() const
        {
          if (_type == Type::Numeric) return decorator::right;
          if (_type == Type::Center) return decorator::center;
          return decorator::left;
        }
        
        constexpr bool isCenter() const { return _type == Type::Center; }
        constexpr bool isNumeric() const { return _type == Type::Numeric || _type == Type::Alpha; }
        Configuration& center() { _type = Type::Center; return *this; }

        constexpr size_t width() const { return _width; }
        Configuration& width(size_t w) { _width = w; return *this; }

        void setNumeric(bool numeric ) 
        {
           // Don't change type if it's already Center or Numeric (when turning off)
           if (_type == Type::Center) return;
           if (!numeric && _type == Type::Numeric) return;  // Preserve numeric when adding non-numeric cells
           _type = not numeric ? Type::Alpha
                               : _type == Type::Default or _type == Type::Numeric ? Type::Numeric : Type::Alpha;
        }

        Configuration& apply(decorator::Tool pre) { 
          _decorators[static_cast<size_t>(Order::Pre)].emplace_back(pre); 
          return *this;
        }
        
        Configuration& apply(Order order, decorator::Tool pre) 
        { 
          _decorators[static_cast<size_t>(order)].emplace_back(pre); 
          return *this;
        }
        
        Configuration& apply(decorator::Tool pre, decorator::Tool post) 
        { 
          _decorators[static_cast<size_t>(Order::Pre)].emplace_back(pre); 
          _decorators[static_cast<size_t>(Order::Post)].emplace_back(post); 
          return *this;
        }

        void apply(Order order, std::ostream& stream, const std::string& str ) const
        {
          const auto& vec = _decorators[static_cast<size_t>(order)];
          for(const auto& decorator : vec)
          {
            decorator(stream, str);
          }
        }
      };

     Configuration& column(size_t index) { return _col[index]; }

     void config(Configuration::Order order, std::ostream& stream, const std::string value) 
     { 
      _row.apply(order, stream, value); 
     }

     void apply(int depColumn, decorator::Tool pre, decorator::Tool post) 
     {
       _depColumn = depColumn; 
       _row.apply(pre, post); 
     }

      int depdentColumn() const { return _depColumn; }

    private:

      std::string                     _sep;     // Separator
      std::string                     _indent;  // Indentation
      std::unordered_map<size_t, Configuration> _col;     // Index -> size
      Configuration                   _row;
      int                             _depColumn = -1;
  };


  /**
   * One field of information abstraction
   */
  class Cell
  {
    public:
      using ConfFunc = std::function<Level::Configuration&()>;

      Cell(const std::string& value, ConfFunc getConf)
      : _confPtr(nullptr), _value(value), _config(getConf), _justifyTool(getConf().justify())
      {
        auto& conf = getConfig();
        conf._width = std::max(conf._width, value.size());
      }

      const std::string& value() const { return _value; }

      size_t size() const { return getConfig()._width; }
      std::ostream& justify(std::ostream& os) const { return _justifyTool(os, _value); }

      void display(std::ostream& stream) const
      {
        if (getConfig().visible())
        {
          if (getConfig().isCenter()) {
            // Handle center alignment specially
            size_t width = size();
            size_t strLen = _value.size();
            if (width > strLen) {
              size_t totalPad = width - strLen;
              size_t leftPad = totalPad / 2;
              size_t rightPad = totalPad - leftPad;
              stream << std::string(leftPad, ' ');
              stream << _value;
              stream << std::string(rightPad, ' ');
            } else {
              stream << _value;
            }
          } else {
            pre(stream);
            stream  << _value; 
            post(stream);
          }
        }
      }

      void isNumeric(bool numeric) { getConfig().setNumeric(numeric); }

    private:
      mutable const Level::Configuration* _confPtr;  // Cached pointer
      std::string  _value;
      ConfFunc     _config;
      decorator::Tool _justifyTool;  // Cached justify function

      Level::Configuration& getConfig() const {
        if (!_confPtr) {
          _confPtr = &_config();
        }
        return const_cast<Level::Configuration&>(*_confPtr);
      }

      void pre(std::ostream& os) const
      {
        getConfig().apply(Level::Configuration::Order::Pre, os, _value);
        os << std::setw(size());
        justify(os); 
      }

      void post(std::ostream& os) const
      {
        getConfig().apply(Level::Configuration::Order::Post, os, _value);
      }
  
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
        add(std::to_string(value), true /* Numeric */);
      }

      void add(const char* value) { 
        if (value) add(std::string(value)); 
      }
      
      Cell& add(const std::string& value, bool numeric = false)
      {
        return add(std::string(value), numeric);
      }

      Cell& add(std::string&& value, bool numeric = false)
      {
        trim(value);
        
        // Note: Using lambda that captures idx - safe because vector doesn't invalidate on push_back
        size_t idx = _cell.size();
        _cell.emplace_back(std::move(value), [this, idx]() -> Level::Configuration& { return _level.column(idx); });
        Cell& cell = _cell.back();
        cell.isNumeric(numeric);

        return cell;
      }

      void add(const literal::FreeStyle& f)
      {
        size_t idx = _cell.size();
        _cell.emplace_back(f._str, [this, idx]() -> Level::Configuration& { 
          return _level.column(idx);
        });
      }

      // TODO: Handle rvalue references, for increase efficiency.

      void display(std::ostream& stream) const
      {
        if (_cell.size())
        {
          stream << _level.indent();
          std::string depValue {  (_level.depdentColumn() == -1) ? "" :  _cell[ static_cast<size_t>(_level.depdentColumn()) ].value() };

          _level.config(Level::Configuration::Order::Pre, stream, depValue );
          for(auto& cell : _cell)
          {
            cell.display(stream);
            stream << _level.separator();
          }
          _level.config(Level::Configuration::Order::Post, stream, depValue );
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
      Surge(std::ostream& out, const std::vector<std::string>& i_names)
      :_current(_level.end()), _out(out)
      {
        title(i_names);
      }

      Surge(): Surge(std::cout, {}) {}

      Surge(std::initializer_list<std::string> i_names  )
      :Surge(std::cout, i_names)
      {
        title(i_names);
      }

      ~Surge() { flush(); }

      void flush()
      {
        // Support Titles only for first Level.
        if (_current != _level.end() and _title) { _title(_out, *_current); } 
        for(auto& row : _row) { row.display(_out); }
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
        if (_row.size())
        {
          _row.pop_back();
        }
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
          if(names.size() > 0)
          {
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
          }
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
