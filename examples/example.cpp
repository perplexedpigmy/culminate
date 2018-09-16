#include "culminate.h"
#include "rang.hpp"

using namespace culminate::literal;
using namespace std;
int main()
{
   culminate::Surge s;
  s.title({"num", "A very very very long title", "1234", "123", "xxx"});
  s.level(1).indent(10);
  s.level(0).column(1).apply([](ostream& os, const string& s) -> ostream& { return culminate::decorator::center(os, s); });

#if 0
  s.level(0).column(2).apply( culminate::decorator::attribute(rang::fg::green) );
  s.level(0).column(3).apply( culminate::decorator::attribute(rang::fg::reset) );
#endif

for (size_t i =0; i< 1000000; ++i)
{
   s << 1 << 1 << "x2.5" << string("adsfdsfbc") << endl;
   s << 2 << 1 << 2.5 << string("abc") << endl;
   s << 3 << 1454 << 2.5 << string("adfbc") << endl;
   s << culminate::Level::next;
//   s << endl;
//   s << "Some text dsfasdfsdfasdfasdf"_free << "dsfasdfsd"_free << endl;
//   s << endl;
   s << 4 << 45341 << 2.5 << string("a") << endl;
   s << "4.5" << 145435 << 2.5 << string("a") << endl;
   s << culminate::Level::prev;
   s << 5 << 1 << 2.5 << string("a") << endl;
}
   return 0;
}
