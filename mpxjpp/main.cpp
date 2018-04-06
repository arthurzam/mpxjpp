#include <iostream>
#include "common/calendar.h"
using namespace std;
#include "projectfile.h"

#include <chrono>

#include "deps/date.h"

using namespace mpxjpp;
using namespace mpxjpp::common;

int main()
{
//  union {
//      mpxjpp::common::DateTime t;
//      int n;
//  }d;
//  d.t = {10, 10, 10};
//  cout << hex << d.n << endl << static_cast<int>(d.t) << endl << endl;
//  d.t = {10, 10, 20};
//  cout << d.n << endl << static_cast<int>(d.t) << endl << endl;
//  static_assert(sizeof(double) <= sizeof(void *), "a");
//  cout << sizeof(double) << sizeof(void *) << endl;
//  cout << "Hello World!" << endl << endl;

//  std::vector<duration<int64_t>> v;
//  v.push_back(minutes(200));
//  v.push_back((1998_y/jan));


    mpxjpp::common::any a;
    cout << a.get<int>(1) << endl;
    a = 2;
    cout << a.get<int>(1) << endl << endl << std::dec;

    cout << sizeof(unsigned) << endl;
    cout << sizeof(std::string) << endl;
    cout << sizeof(mpxjpp::Duration) << endl;
    cout << sizeof(std::shared_ptr<std::string>) << endl;

//  year_month_day i = 1998_y/jan/1_d;
//  auto j = static_cast<sys_days>(i) + seconds(5);
//  local_days d(i);
//  d.time_since_epoch().count();

//    cout << i << endl << normalize(2016_y/mar/0) << endl;

    return 0;
}
