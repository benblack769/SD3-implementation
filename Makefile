# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


#SD3: Point.o PendingPointTable.o Stride.o PendingStrideTable.o StrideDetector.o tester.o 
#	g++ -o sd3 Point.o PendingPointTable.o Stride.o PendingStrideTable.o StrideDetector.o tester.o

SD3: StrideDetector.o PendingStrideTable.o PendingPointTable.o Stride.o Point.o tester.o 
	g++ -g -o sd3 StrideDetector.o PendingStrideTable.o PendingPointTable.o Stride.o Point.o tester.o

Point.o: Types.h Point.h  

Stride.o: Types.h Stride.h  

PendingStrideTable.o: Types.h Stride.h PendingStrideTable.h  

PendingPointTable.o: Types.h Point.h PendingPointTable.h  

StrideDetector.o: StrideDetector.h  

tester.o: Types.h Point.h Stride.h PendingStrideTable.h PendingPointTable.h StrideDetector.h  

clean:
	/bin/rm -f sd3 *.o core


