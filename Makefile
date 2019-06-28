
INPUTVECTOR_DIR = lib/inputvectors

main-default: main.cpp default-inputvector
	g++ -o main-default main.cpp $(INPUTVECTOR_DIR)/defaultvector.o -lpthread -I. 

main-updown: main.cpp updown-inputvector
	g++ -o main-updown main.cpp $(INPUTVECTOR_DIR)/updownvector.o -lpthread -I. 

default-inputvector: $(INPUTVECTOR_DIR)/defaultvector.cpp
	g++ -c $(INPUTVECTOR_DIR)/defaultvector.cpp -I. -o $(INPUTVECTOR_DIR)/defaultvector.o

updown-inputvector: $(INPUTVECTOR_DIR)/updownvector.cpp
	g++ -c $(INPUTVECTOR_DIR)/updownvector.cpp -I. -o $(INPUTVECTOR_DIR)/updownvector.o




testmain: testmain.cpp testEmitter testCollector
	g++ -o testmain testmain.cpp

testEmitter: tests/testEmitter.cpp
	g++ -c tests/testEmitter.cpp -I.

testCollector: tests/testCollector.cpp
	g++ -c tests/testCollector.cpp -I.

all: main-default main-updown


clean:
	rm main-default main-updown $(INPUTVECTOR_DIR)/defaultvector.o $(INPUTVECTOR_DIR)/updownvector.o