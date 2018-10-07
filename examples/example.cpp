#include "culminate.h"
#include "rang.hpp"

using namespace culminate::literal;
using namespace std;
using namespace rang;


int main()
{

   culminate::Surge s;
  s.title({"num", "A very very very long title", "1234", "123", "xxx"});
  s.level(1).indent(10);

  s.level(0).column(3).apply(culminate::decorator::center);
  s.level(0).column(0).apply(culminate::decorator::center);
  s.level(0).column(1).apply(culminate::decorator::left)
                      .apply(culminate::decorator::conditionalCode( 
                        [](int value) { return value > 10; }, fg::green, fg::red),
                        culminate::decorator::code(rang::style::reset));
  s.level(0).column(2).apply(culminate::decorator::center);

  s.level(1).apply(1, culminate::decorator::conditionalCode(
                   [](int value) { return value > 45341; }, fg::green, fg::red),
                   culminate::decorator::code(rang::style::reset));

   s << 1 << 1 << "x2.5" << string("adsfdsfbc") << endl;
   s << 2 << 10 << 2.5 << string("abc") << endl;
   s << 3 << 1454 << 2.5 << string("adfbc") << endl;
   s << 234 << 1 << "abc" << endl;
 
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
