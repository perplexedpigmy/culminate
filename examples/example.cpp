#include "culminate.h"
#include "rang.hpp"
#include <set>
#include <string>
#include <map>


using namespace culminate::literal;
using namespace std;
using namespace rang;


int main()
{
  {
   culminate::Surge s;
  s.title({"num", "A very very very long title", "1234", "123", "xxx"});
  s.level(1).indent(10);

  s.level(1).column(4).hide();
  s.level(0).column(3).apply(culminate::decorator::center);
  s.level(0).column(0).apply(culminate::decorator::center);
  s.level(0).column(1).apply(culminate::decorator::left)
                      .apply(culminate::decorator::conditionalCode( 
                        [](int value) { return value > 10; }, fg::green, fg::red),
                        culminate::decorator::code(rang::style::reset));
  s.level(0).column(2).apply(culminate::decorator::center);

  s.level(1).apply(1, culminate::decorator::conditionalCode(
                   [](int value) { return value > 0; }, fg::green, fg::red),
                   culminate::decorator::code(rang::style::reset));

   s << 1 << 1 << "x2.5" << string("adsfdsfbc") << endl;
   s << 2 << 10 << 2.5 << string("abc") << endl;
   s << 3 << 1454 << 2.5 << string("adfbc") << endl;
   s << 234 << 1 << "abc" << endl;
 
//   s << endl;
//   s << "Some text dsfasdfsdfasdfasdf"_free << "dsfasdfsd"_free << endl;
//   s << endl;
   s << culminate::Level::next;
   s << 4 << 45341 << 2.5 << string("a")    << 0<< endl;
   s << 4.5 << 145435 << 2.5 << string("a") << 1 << endl;
   s << culminate::Level::prev;
   s << 10 << 345100 << 2.5 << string("a") << endl;

   }
   {
   culminate::Surge s{"   1    ", "    2    "};
//    s.level(0).apply(1, culminate::decorator::conditionalCode(
//                   [](int value) { return value > 0; }, fg::gray, fg::red),
//                   culminate::decorator::code(rang::style::reset));
    s.level(1).apply(4, culminate::decorator::conditionalCode(
                   [](int value) { return value > 0; }, fg::gray, fg::red),
                    culminate::decorator::code(rang::style::reset));
    s.level(0).column(1).hide();
    s.level(1).column(4).hide();


   map<string, map<size_t,string>> lf { {"DR1",{ { 1, "abc" }, { 2, "def" } } },
                                         {"DR2",{ { 1, "abc" }               } },
                                         {"DR4",{               { 2, "def" } } },
                                         {"DR5",{                            } },
                                         {"DR6",{               { 2, "def" } } } };

   map<string, map<size_t,string>> rt { {"DR1",{ { 1, "abc" }, { 2, "def" } } },
                                        {"DR2",{               { 2, "def" } } },
                                        {"DR3",{ { 1, "abc" }, { 2, "def" } } },
                                        {"DR4",{               { 2, "dxf" } } },
                                        {"DR5",{                            } } };
   set<string> keys;
   for (const auto& e : lf) { keys.emplace(e.first); }
   for (const auto& e : rt) { keys.emplace(e.first); }

   for (const auto& key : keys)
   {
      auto lItr = lf.find(key);
      auto rItr = rt.find(key);

      s << key
        << (lItr != lf.end() and rItr != rt.end()) << endl << culminate::Level::next;

      set<size_t> piKeys;
      for (const auto& e : lf[key]) { piKeys.emplace(e.first); }
      for (const auto& e : rt[key]) { piKeys.emplace(e.first); }

      for (const auto& piKey : piKeys)
      {
        auto lpiItr = lf[key].find(piKey);
        auto rpiItr = rt[key].find(piKey);

        if (lpiItr == lf[key].end())
        {
          s << "" << ""; 
        }
        else 
        {
          s << lpiItr->first << lpiItr->second; 
        }

        if (rpiItr== rt[key].end())
        {
          s << "" << "";
        }
        else 
        {
          s << rpiItr->first << rpiItr->second; 
        }

        s << (lpiItr != lf[key].end() and rpiItr != rt[key].end() and lpiItr->first == rpiItr->first and lpiItr->second == rpiItr->second) << endl;
      }

      s << culminate::Level::prev;
   }

   } 

   return 0;
}
