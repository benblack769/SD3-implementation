#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "PendingPointTable.h"
#include "PendingStrideTable.h"
#include "Stride.h"
#include "StrideDetector.h"
#include <stdlib.h>

using namespace std;

PendingStrideTable *pendingStrideTable = NULL;
PendingPointTable *pendingPointTable = NULL;
map<long long, StrideDetector *> stridePerPCMap; // "Stride detector per PC" map

void handleStrideDetectorResult(long long pc, long long memAddr,
                                MemAccessType result, StrideDetector *strDet) {
    // This method assumes the pendingTables have already been initialized

    if (result == UNKNOWN) {
        // Do nothing

    } else if (result == LOSTPOINT) {
        // Need to get previous last points and create points for them.
        // This occurs when a stride hasn't actually been learned yet so the
        // last two accesses need to be accounted for.

        long long lost = strDet->getFirstLostPoint();
        if (pendingPointTable->doesPointExist(lost, pc, READ)) {
            pendingPointTable->updateExistingPoint(lost, pc, READ);
        } else {
            pendingPointTable->addNewPoint(lost, pc, 1, 4, READ);
        }

        long long lost2 = strDet->getSecondLostPoint();
        if (lost2 != 0) {
            if (pendingPointTable->doesPointExist(lost2, pc, READ)) {
                pendingPointTable->updateExistingPoint(lost2, pc, READ);
            } else {
                pendingPointTable->addNewPoint(lost2, pc, 1, 4, READ);
            }
        }

    } else if (result == NEWPOINT) {
        // This happens when there have been 3 consecutive accesses to this
        // point (stride==0) We need to account for all three accesses when
        // adding the point
        if (pendingPointTable->doesPointExist(memAddr, pc, READ)) {
            pendingPointTable->updateExistingPoint(memAddr, pc, READ);
            pendingPointTable->updateExistingPoint(memAddr, pc, READ);
            pendingPointTable->updateExistingPoint(memAddr, pc, READ);
        } else {
            pendingPointTable->addNewPoint(memAddr, pc, 3, 4, READ);
        }

    } else if (result == POINT) {
        // This would occur if the current access did not meet the current
        // stride, making the StrideDetector go back from a state where the
        // stride was learned.
        if (pendingPointTable->doesPointExist(memAddr, pc, READ)) {
            pendingPointTable->updateExistingPoint(memAddr, pc, READ);
        } else {
            pendingPointTable->addNewPoint(memAddr, pc, 1, 4, READ);
        }
    } else if (result == STRIDE) {
        int strideLength = strDet->getStride();
        if (pendingStrideTable->doesStrideExist(
                pc, memAddr, strideLength,
                READ)) { // just hard coding R/W for now
            pendingStrideTable->updateExistingStride(pc, memAddr, strideLength,
                                                     READ);
        } else {
            pendingStrideTable->addNewStride(
                pc, strDet->getFirstMemAddr(), strDet->getPrevMemAddr(),
                strideLength, 3, 4,
                READ); // hard coding R/W and access Size for testing
        }
    } else {
        cerr << "RESULT FOR " << hex << memAddr << dec << " is UNDEFINED"
             << endl;
        exit(1);
    }
}

/* This should be called after all inputs have been processed.  It goes through
 * all of the StrideDetectors and sees if they are in any state that might
 * indicate that a previous access has not been propagated either to the
 * PendingStrideTable or PendingPointTable. Since anything classified as a
 * Stride is automatically put into the PendingStrideTable, anything left in a
 * Stride Detector will be viewed as a Point and will be added to the
 * PendingPointTable.
 */
void purgeStrideDetectorState() {
    map<long long, StrideDetector *>::iterator iter;

    cout << "PURGE # of entries: " << stridePerPCMap.size() << endl;

    for (iter = stridePerPCMap.begin(); iter != stridePerPCMap.end(); iter++) {
        long long pc = iter->first;
        StrideDetector *strDet = iter->second;

        cout << "Purge PC : " << pc << " ";
        // Check to see if the StrideDetector is in a state that implies there
        // are non-propagated values.  flush() returns true if that is the case.
        // We refer to these points as "lost points".
        if (strDet->flush()) {
            cout << "lost points: ";
            long long lost = strDet->getFirstLostPoint();
            cout << lost << " ";
            if (pendingPointTable->doesPointExist(lost, pc, READ)) {
                pendingPointTable->updateExistingPoint(lost, pc, READ);
            } else {
                pendingPointTable->addNewPoint(lost, pc, 1, 4, READ);
            }

            long long lost2 = strDet->getSecondLostPoint();
            if (lost2 != 0) {
                cout << lost2 << endl;
                if (pendingPointTable->doesPointExist(lost2, pc, READ)) {
                    pendingPointTable->updateExistingPoint(lost2, pc, READ);
                } else {
                    pendingPointTable->addNewPoint(lost2, pc, 1, 4, READ);
                }
            } else {
                cout << endl;
            }
        } else {
            cout << " nothing lost " << endl;
        }
    }
}

/* isStrideDetectInstPresent
 *
 * Takes in stridePerPCMap (from main) as the argument for 1st parameter and the
 * current PC address as the argument for the 2nd parameter Determines if PCAdd
 * is already present in the map If present, it returns true; else it returns
 * false
 */

bool isStrideDetectInstPresent(
    map<long long, StrideDetector *> &strideDetectorInstMap, long long PCAdd) {
    map<long long, StrideDetector *>::iterator it =
        strideDetectorInstMap.begin();

    it = strideDetectorInstMap.find(PCAdd);

    if (it != strideDetectorInstMap.end()) {
        // use the existing PC's stride detector instance
        return true;
    } else {
        // need to create new stride detector instance to store information
        // need to create a new element in map (PC = key; stride detector
        // instance = value)
        return false;
    }
}

/* Main method
 *
 * Reads in values from the file and passes them as agruments for the memAdd
 * parameter in PointOrStride method. Stores the current phase of the FSM and
 * passes it as an argument for the recursive call of the PointOrStride method.
 * Prints out the pending tables for both point addresses and stride addresses.
 */

int main() {
    long long fileLine;
    ifstream myfile("memAddFile2.txt");

    long long PCAdd = 0;
    long long prevPC;

    MemAccessType currentResult = UNKNOWN;

    StrideDetector
        *strideDetectorPerPC; // Used only for the very first PC address

    pendingStrideTable = new PendingStrideTable();
    pendingPointTable = new PendingPointTable();

    if (myfile.is_open()) {
        while (myfile >> fileLine) {
            if (PCAdd != 0) {
                if (isStrideDetectInstPresent(stridePerPCMap, PCAdd) == false) {
                    strideDetectorPerPC =
                        new StrideDetector(); // local stride detector instance
                    cout << " Inserting new PCAdd " << PCAdd << endl;
                    stridePerPCMap[PCAdd] = strideDetectorPerPC;
                } else {
                    strideDetectorPerPC = stridePerPCMap[PCAdd];
                }
            }

            if (fileLine >
                400000) // If line is a PC address, then store it in PC
                        // address variable //
            {
                prevPC =
                    PCAdd; // used to check if PCAdd is the first PCAdd or not

                PCAdd = fileLine; // storing new PC address into PCAdd
                cout << "PC: " << PCAdd << endl;
            } else {
                long long memAddr = fileLine;
                currentResult = strideDetectorPerPC->addAccess(memAddr);
                handleStrideDetectorResult(PCAdd, memAddr, currentResult,
                                           strideDetectorPerPC);
                cout << "Address: " << memAddr << endl;
                cout << "Result: " << currentResult << endl;
                cout << "Current State: " << strideDetectorPerPC->getState()
                     << endl;
                ;
                cout << "Current Stride: " << strideDetectorPerPC->getStride()
                     << endl;
                ;
                cout << endl;
            }
        }

        myfile.close();

    } else {
        cout << "Unable to open file";
    }
    purgeStrideDetectorState();

    cout << "STRIDES: " << endl;
    pendingStrideTable->print();
    cout << "POINTS: " << endl;
    pendingPointTable->print();
}
