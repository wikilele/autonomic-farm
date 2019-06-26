
main: main.cpp 
	g++ -o main main.cpp  -lpthread -I. 

testmain: testmain.cpp testEmitter testCollector
	g++ -o testmain testmain.cpp

testEmitter: tests/testEmitter.cpp
	g++ -c tests/testEmitter.cpp -I.

testCollector: tests/testCollector.cpp
	g++ -c tests/testCollector.cpp -I.


clean:
	rm main testmain testCollector.o testEmitter.o