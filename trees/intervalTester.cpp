
using namespace std;

#include "Interval.h"
#include <iostream>
#include <map>
#include <string>

int main(int argc, char **argv) {
    map<Interval, string, IntervalLessThan> imap;

    Interval i1(5, 8);
    Interval i2(2, 3);
    Interval i3(4, 7);

    imap[i1] = "5-8";
    imap[i2] = "2-3";
    imap[i3] = "4-7";

    for (map<Interval, string>::iterator iter = imap.begin();
         iter != imap.end(); iter++) {
        cout << (*iter).second << endl;
    }
}
