#include "PendingPointTable.h"

// If memory address isn't killed, adds information about this access to table.
// Returns true if information is added, false if memory address is already
// killed.  If this access is a write, the point is turned to "killed"
bool PendingPointTable::addNewPoint(int64_t memAddress, int64_t PC, int64_t numAccesses, int64_t accessSize,
                                    MemAccessMode readOrWrite) {
    // If memory address already killed, do not add new elements.
    map<int64_t, PointEntry>::iterator iter = myPendingPoints.find(memAddress);
    if (iter != myPendingPoints.end()) {
        // Memory address already in table
        if (iter->second.killed == true) {
            return false;
        } else {
            Point *p = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);
            iter->second.points.push_back(p);

            if (readOrWrite == WRITE) {
                iter->second.killed = true;
            }
        }
    } else {
        PointEntry temp;
        temp.points.push_back(new Point(memAddress, PC, numAccesses, accessSize, readOrWrite));

        if (readOrWrite == WRITE) {
            temp.killed = true;
        }

        myPendingPoints.insert(pair<int64_t, PointEntry>(memAddress, temp));
    }

    return true;
}

bool PendingPointTable::doesPointExist(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite) {
    map<int64_t, PointEntry>::iterator iter = myPendingPoints.find(memAddress);

    if (iter != myPendingPoints.end()) {
        for (list<Point *>::iterator liter = iter->second.points.begin(); liter != iter->second.points.end(); liter++) {
            if (PC == (*liter)->getPC() && readOrWrite == (*liter)->getAccessMode()) {
                return true;
            }
        }
    }

    return false;
}

// If memory address already is in table and is not killed, updates number of
// accesses. Returns true if update of number of accesses occurs.
bool PendingPointTable::updateExistingPoint(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite) {
    map<int64_t, PointEntry>::iterator iter = myPendingPoints.find(memAddress);

    if (iter != myPendingPoints.end()) {
        if (iter->second.killed == true) {
            return false;
        }

        for (list<Point *>::iterator liter = iter->second.points.begin(); liter != iter->second.points.end(); liter++) {
            if (PC == (*liter)->getPC() && readOrWrite == (*liter)->getAccessMode()) {
                (*liter)->addAccess();
                cout << "\t AFTER UPDATE: " << endl;
                (*liter)->print();
                cout << endl;
                return true;
            }
        }
    }

    return false;
}

bool PendingPointTable::isPointKilled(int64_t memAddress) {
    map<int64_t, PointEntry>::iterator iter = myPendingPoints.find(memAddress);

    if (iter != myPendingPoints.end()) {
        if (iter->second.killed == true) {
            return true;
        }
    }

    return false;
}

/*
  list<Dependences>
  PendingPointTable::checkForPointDependences(HistoryPointTable ht)
  {
  list<Dependence> conflicts;
  
  map<int64_t, PointEntry>::iterator iter = myPendingPoints.begin();
  for( ; iter!= myPendingParts.end(); iter++){
  for(list<Point*>::iterator liter = iter.second.points.begin(); liter !=
  iter.second.points.end(); liter++){ list<Point *> oldPoints =
  ht->getPoints(liter->getMemAddress());
  
  MemAccessMode pendMode = liter->getAccessMode();
  for(list<Point*>::iterator piter = oldPoints.begin(); piter !=
  oldPoints.end(); piter++){ MemAccessMode oldMode = piter->getAccessMode();
  if(oldMode != READ && pendMode != READ){
  Dependence d(liter->getMemoryAddr(), piter->getPC(), oldMode, liter->getPC(),
  pendMode); conflicts.insert(d);
  }
  
  }
  }
  }
  return conflicts;
  }
*/

/*
  map<Interval, PointEntry> PendingPointTable::getIntervalMap()
  {
  map<Interval, PointEntry, IntervalLessThan> iMap;
  
  map<int64_t, PointEntry>::iterator iter = myPendingPoints.begin();
  
  for( ; iter != myPendingPoints.end(); iter++){
  Interval i(iter.second->getMemoryAddr(), iter.second->getMemoryAddr());
  iMap[i] = iter.second;
  }
  return iMap;
  }
*/

void PendingPointTable::print() {
    map<int64_t, PointEntry>::iterator iter;

    for (iter = myPendingPoints.begin(); iter != myPendingPoints.end(); iter++) {
        list<Point *>::iterator liter;
        for (liter = iter->second.points.begin(); liter != iter->second.points.end(); liter++) {
            cout << "Address: " << iter->first;
            cout << dec << " killed: " << iter->second.killed << " ";
            (*liter)->print();
        }

        cout << "*****" << endl;
    }
}
