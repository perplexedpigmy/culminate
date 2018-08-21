#include "culminate.h"

using namespace std;
int main()
{
   culminate::Surge s;

   s << "abc" << "def" << "me" << endl;
   s << "abc" << 1 << 2.5 << string("adsfdsfbc") << endl;
   s << "ac" << 1 << 2.5 << string("abc") << endl;
   s << "abdsfc" << 1454 << 2.5 << string("adfbc") << endl;
   s << endl;
   s << "Some text dsfasdfsdfasdfasdf"_free << endl;
   s << endl;
   s << "abfdc" << 1 << 2.5 << string("a") << endl;
   s << "abfdcsdfadsfasdfsdf" << 1 << 2.5 << string("a") << endl;
   return 0;
}
