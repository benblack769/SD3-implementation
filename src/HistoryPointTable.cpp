#include "HistoryPointTable.h"

void HistoryPointTable::addNewPoint(int64_t memAddress, int64_t PC, int64_t numAccesses, int64_t accessSize,
                                    MemAccessMode readOrWrite) {
    Point *temp;
    temp = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);

    myPoints.insert(pair<int64_t, Point *>(memAddress, temp));
}

bool HistoryPointTable::doesPointExist(int64_t memAddress) {
    pair<multimap<int64_t, Point *>::iterator, multimap<int64_t, Point *>::iterator> rangeIter;
    multimap<int64_t, Point *>::iterator iter;

    rangeIter = myPoints.equal_range(memAddress);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        return true;
    }

    return false;
}

list<Point *> HistoryPointTable::getPoints(int64_t memAddress) {
    list<Point *> results;

    pair<multimap<int64_t, Point *>::iterator, multimap<int64_t, Point *>::iterator> rangeIter;
    multimap<int64_t, Point *>::iterator iter;

    rangeIter = myPoints.equal_range(memAddress);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        results.insert(iter.second);
    }

    return results;
}

map<Interval, bool> HistoryPointTable::getIntervalMap() {
    map<Interval, bool, IntervalLessThan> iMap;

    multimap<int64_t, Point *>::iterator iter = myPoints.begin();

    for (; iter != myPoints.end(); iter++) {
        Interval i(iter.second->getMemoryAddr(), iter.second->getMemoryAddr);
        iMap[i] = false;
    }
    return iMap;
}
