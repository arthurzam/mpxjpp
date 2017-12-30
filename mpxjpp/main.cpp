#include <iostream>
#include "common/calendar.h"
using namespace std;

int main()
{
	union {
		mpxjpp::common::Date t;
		int n;
	}d;
	d.t = {10, 10, 10};
	cout << hex << d.n << endl << static_cast<int>(d.t) << endl << endl;
	d.t = {10, 10, 20};
	cout << d.n << endl << static_cast<int>(d.t) << endl << endl;
	static_assert(sizeof(double) <= sizeof(void *), "a");
	cout << sizeof(double) << sizeof(void *) << endl;
	cout << "Hello World!" << endl;
	return 0;
}
