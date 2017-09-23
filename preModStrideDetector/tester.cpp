#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include "StrideDetector.h"

using namespace std;

/* isStrideDetectInstPresent
*
* Takes in stridePerPCMap (from main) as the argument for 1st parameter and the current PC address as the argument
* for the 2nd parameter
* Determines if PCAdd is already present in the map
* If present, it returns true; else it returns false
*/

bool isStrideDetectInstPresent(map<long long, StrideDetector*> & strideDetectorInstMap, long long PCAdd)
{
	map<long long, StrideDetector*>::iterator it = strideDetectorInstMap.begin();

	it = strideDetectorInstMap.find(PCAdd);

	if(it != strideDetectorInstMap.end())
	{
		// use the existing PC's stride detector instance
		return true;
	}
	else
	{
		// need to create new stride detector instance to store information
		// need to create a new element in map (PC = key; stride detector instance = value)
		return false;
	}
}


/* Main method
* 
* Reads in values from the file and passes them as agruments for the memAdd parameter in PointOrStride method.
* Stores the current phase of the FSM and passes it as an argument for the recursive call of the PointOrStride method.
* Prints out the pending tables for both point addresses and stride addresses. 
*/

int main()
{
    long long fileLine;
    ifstream myfile ("memAddFile2.txt");

    long long PCAdd = 0;
    long long prevPC;

    map<long long, StrideDetector*> stridePerPCMap; // "Stride detector per PC" map

    int currentResult = UNKNOWN;

    StrideDetector *strideDetectorPerPC; // Used only for the very first PC address
    
    if( myfile.is_open() )
    {
        while( myfile >> fileLine )
        {	   
	    if(isStrideDetectInstPresent(stridePerPCMap, PCAdd) == false)
	    {
	      strideDetectorPerPC = new StrideDetector(); // local stride detector instance
	      stridePerPCMap[PCAdd] = strideDetectorPerPC;
	    }
	    else{
	      strideDetectorPerPC = stridePerPCMap[PCAdd];
	    }
	   
            if(fileLine > 400000) // If line is a PC address, then store it in PC address variable // 
	    {
		prevPC = PCAdd; // used to check if PCAdd is the first PCAdd or not

		PCAdd = fileLine; // storing new PC address into PCAdd
		cout <<"PC: " << PCAdd << endl;
	    }
            else
            {
	      currentResult = strideDetectorPerPC->addAccess(fileLine);
	      cout <<"Address: " << fileLine << endl;
	      cout <<"Result: " << currentResult << endl;
	      cout <<"Current State: " << strideDetectorPerPC->getState() << endl;;
	      cout <<"Current Stride: " << strideDetectorPerPC->getStride() << endl;;
	      cout << endl;
            }
        }

       myfile.close();

    }
    else
    {
        cout << "Unable to open file";
    }


}
