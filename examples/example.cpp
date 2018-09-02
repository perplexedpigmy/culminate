#include "culminate.h"

using namespace culminate::literal;
using namespace std;
int main()
{
   culminate::Surge s;

  s.title({"1234", "1234xy", "1234", "123", "xxx"});
 
   s << "1" << 1 << 2.5 << string("adsfdsfbc") << endl;
   s << "2" << 1 << 2.5 << string("abc") << endl;
   s << "3" << 1454 << 2.5 << string("adfbc") << endl;
   s << culminate::Level::next;
//   s << endl;
//   s << "Some text dsfasdfsdfasdfasdf"_free << "dsfasdfsd"_free << endl;
//   s << endl;
   s << "4" << 45341 << 2.5 << string("a") << endl;
   s << "4.5" << 145435 << 2.5 << string("a") << endl;
   s << culminate::Level::prev;
   s << "5" << 1 << 2.5 << string("a") << endl;
   return 0;
}
