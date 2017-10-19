#include <iostream>

#include "HistoryPointTable.h"
#include "Types.h"

using namespace std;

HistoryPointTable fill_table() {
    HistoryPointTable table;
    int add_start = 4000000;
    int PC = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 100; j += 10) {
            table.addNewPoint(i + add_start, PC);
            PC++;
        }
    }
    return table;
}
void print_points(list<Point *> points) {
    for (list<Point *>::iterator iter = points.begin(); iter != points.end(); iter++) {
        (*iter)->print();
    }
}

void test() {
    HistoryPointTable table1 = fill_table();
    print_points(table1.getPoints(3));
}

int main() { test(); }
