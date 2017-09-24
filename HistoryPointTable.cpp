#include "HistoryPointTable.h"

void HistoryPointTable::addNewPoint(long long memAddress, long long PC, long long numAccesses, long long accessSize,
                                    MemAccessMode readOrWrite) {
    Point *temp;
    temp = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);

    myPoints.insert(pair<long long, Point *>(memAddress, temp));
}

bool HistoryPointTable::doesPointExist(long long memAddress) {
    pair<multimap<long long, Point *>::iterator, multimap<long long, Point *>::iterator> rangeIter;
    multimap<long long, Point *>::iterator iter;

    rangeIter = myPoints.equal_range(memAddress);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        return true;
    }

    return false;
}

list<Point *> HistoryPointTable::getPoints(long long memAddress) {
    list<Point *> results;

    pair<multimap<long long, Point *>::iterator, multimap<long long, Point *>::iterator> rangeIter;
    multimap<long long, Point *>::iterator iter;

    rangeIter = myPoints.equal_range(memAddress);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        results.insert(iter.second);
    }

    return results;
}

map<Interval, bool> HistoryPointTable::getIntervalMap() {
    map<Interval, bool, IntervalLessThan> iMap;

    multimap<long long, Point *>::iterator iter = myPoints.begin();

    for (; iter != myPoints.end(); iter++) {
        Interval i(iter.second->getMemoryAddr(), iter.second->getMemoryAddr);
        iMap[i] = false;
    }
    return iMap;
}
