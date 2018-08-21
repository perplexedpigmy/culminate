#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>


namespace culminate
{
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
   * One field of information abstraction
   */
  class Cell
  {
    public:

      Cell(const std::string& value, size_t& size)
      : _value(value), _size(size), _side(std::left) {}
      size_t size() const {  return _size; }
      const std::string& value() const { return _value; }
      justification side() const { return _side; }

    private:
      std::string   _value;
      size_t&       _size;
      justification _side;
  };

  /**
   * Used as configuration class for an entire idnetiation level
   * Including but not restircted to max column max size.
   **/
  class Level
  {
    public:
      Level(size_t indentSize = 0, const std::string& sep = "  ")
      : _indent(indentSize, ' '), _sep(sep) {}

      const std::string& separator() const { return _sep; }
      const std::string& indent() const { return _indent; }

      size_t& columnSize(size_t index, size_t newSize)
      {
        return _col[index] = std::max(newSize, _col[index]);
      }


    private:
      std::string _sep;           // Separator
      std::string _indent;        // Indentation
      std::map<size_t, size_t> _col;   // Index -> size
  };

  /**
   * A line contains 1-n Cells.
   **/
  class Line
  {
    public:
      Line(Level& level): _level(level) {}

      template <typename T>
      void add(const T& value) { add(std::to_string(value)); }
      void add(const char* value) { add(std::string(value)); }
      void add(const std::string& value)
      {
        string trimmed(value);
        trim(trimmed);
        _cell.emplace_back(trimmed, _level.columnSize(_cell.size(), trimmed.size() ) );
      }

      // TODO: Handle rvalue references, for increase efficiency.

      void display(std::ostream& stream) const
      {
        stream << _level.indent();
        for(auto& cell : _cell)
        {
          stream << cell.side() << std::setw( cell.size() ) << cell.value() << _level.separator();
        }
        stream << "\n";
      }

    private:
      std::vector<Cell> _cell;
      Level&            _level;
  };

  /**
   * A heavy duty stream
   */
  class Surge
  {
    public:
      Surge(std::ostream& out = std::cout)
      : _out(out), _currentLevel(0)
      {
        _level.reserve(10);
        _line.reserve(20000);
      }

      ~Surge() { flush(); }
     void flush()
     {
       for(auto& line : _line)
       {
         line.display(_out);
       }
       _line.clear();
     }

      template <typename T>
      Surge& operator<<(const T& value)
      {
        line().add(value);
        return *this;
      }

      Surge& operator<<(manipulator m)
      {
        if (m == static_cast<manipulator>(std::endl))
        {
          _line.emplace_back(level());
        }
        else if (m == static_cast<manipulator>(std::ends))
        {
          flush();
        }
        return *this;
      }


      Line& line()
      {
        if (_line.empty())
        {
          _line.emplace_back(level());
        }
        return _line.back();
      }

      Level& level()
      {
        if (_level.size() <= _currentLevel)
        {
          _level.emplace_back();
        }
        return _level[_currentLevel];
      }

    private:
      std::vector<Line>   _line;
      std::vector<Level>  _level;
      std::ostream&       _out;
      size_t              _currentLevel;
  };
};
