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

      size_t& columnSize(size_t index, size_t newSize); 


    private:
      std::string _sep;           // Separator
      std::string _indent;        // Indentation 
      std::map<size_t, size_t> _col;   // Index -> size
  };

  size_t& 
  Level::columnSize(size_t index, size_t newSize) 
  {
    return _col[index] = std::max(newSize, _col[index]);
  }

  class Line
  {
    public:
      Line(Level& level): _level(level) {}

      template <typename T>
      void add(const T& value);
      void add(const char* value);
      void add(const std::string& value);

      // TODO: Handle rvalue reerences 

      void display(std::ostream& stream) const;

    private:
      std::vector<Cell> _cell;
      Level&            _level;
  };

  void Line::add(const std::string& value)
  {
    _cell.emplace_back(value, _level.columnSize(_cell.size(), value.size() ) );
  }
  
  void Line::add(const char* value)
  {
     add(std::string(value));
  }

  template <typename T>
  void Line::add(const T& value)
  {
    add(std::to_string(value));
  }

  void Line::display(std::ostream& stream) const
  {
    stream << _level.indent();
    for(auto& cell : _cell)
    {
      stream << cell.side() << std::setw( cell.size() ) << cell.value() << _level.separator();
    } 
    stream << "\n";
  }

  class Surge
  {
    public:
      Surge(std::ostream& out = std::cout)
      : _out(out), _currentLevel(0) 
      {
        _level.reserve(10);
      }

      ~Surge() { flush(); }
     void flush();

      template <typename T>
      Surge& operator<<(const T& value);

      Surge& operator<<(manipulator m);


      Line& line();
      Level& level();

    private:
      std::vector<Line>   _line;
      std::vector<Level>  _level;
      std::ostream&       _out;
      size_t              _currentLevel;
  };

  template <typename T>
  Surge& Surge::operator<<(const T& value)
  {
     line().add(value);
     return *this;
  }

  Surge& Surge::operator<<(manipulator m)
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

  Line& Surge::line()
  {
    if (_line.empty())
    {
      _line.emplace_back(level());
    }
    return _line.back();
  }

  Level& Surge::level()
  {
    if (_level.size() <= _currentLevel)
    {
      _level.emplace_back();
    }
    return _level[_currentLevel];
  }

  void 
  Surge::flush() 
  {
    for(auto& line : _line)
    {
      line.display(_out);
    }
  }
};


