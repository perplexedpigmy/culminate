#include "culminate.h"
#include "rang.hpp"

using namespace culminate::literal;
using namespace std;
int main()
{
   culminate::Surge s;
  s.title({"num", "A very very very long title", "1234", "123", "xxx"});
  s.level(1).indent(10);

  s.level(0).column(3).apply(culminate::decorator::center);
  s.level(0).column(0).apply(culminate::decorator::center);
  s.level(0).column(1).apply(culminate::decorator::center)
                      .apply([](ostream& os, const string& s) -> ostream&  {
    if (stoi(s) > 5 ) { size_t sz(os.width());  os.width(0); os << rang::fg::green; os.width(sz); }
    return os;
  });
  s.level(0).column(2).apply(culminate::decorator::center)
                      .apply(culminate::decorator::code(rang::style::reset));

   s << 1 << 1 << "x2.5" << string("adsfdsfbc") << endl;
   s << 2 << 10 << 2.5 << string("abc") << endl;
   s << 3 << 1454 << 2.5 << string("adfbc") << endl;
 
   s << culminate::Level::next;
//   s << endl;
//   s << "Some text dsfasdfsdfasdfasdf"_free << "dsfasdfsd"_free << endl;
//   s << endl;
   s << 4 << 45341 << 2.5 << string("a") << endl;
   s << 4.5 << 145435 << 2.5 << string("a") << endl;
   s << culminate::Level::prev;
   s << 10 << 345100 << 2.5 << string("a") << endl;
   return 0;
}
