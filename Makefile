BUILD_DIR = build
INPUTVECTOR_DIR = lib/inputvectors
DEFAULT_VEC = $(INPUTVECTOR_DIR)/defaultvector
UPDOWN_VEC = $(INPUTVECTOR_DIR)/updownvector
HALFDEFAULT_VEC = $(INPUTVECTOR_DIR)/halfdefaultvector
CONSTANT_VEC = $(INPUTVECTOR_DIR)/constantvector
HIGHLOW_VEC = $(INPUTVECTOR_DIR)/highlowvector
LOWHIGH_VEC = $(INPUTVECTOR_DIR)/lowhighvector


# each main-* file gets a different vector in input

main-default: main.cpp default-inputvector
	g++ -o $(BUILD_DIR)/main-default main.cpp $(BUILD_DIR)/$(DEFAULT_VEC).o -lpthread -I. 

main-updown: main.cpp updown-inputvector
	g++ -o $(BUILD_DIR)/main-updown main.cpp $(BUILD_DIR)/$(UPDOWN_VEC).o -lpthread -I. 

main-halfdefault: main.cpp halfdefault-inputvector
	g++ -o $(BUILD_DIR)/main-halfdefault main.cpp $(BUILD_DIR)/$(HALFDEFAULT_VEC).o -lpthread -I. 

main-constant:  main.cpp constant-inputvector
	g++ -o $(BUILD_DIR)/main-constant main.cpp $(BUILD_DIR)/$(CONSTANT_VEC).o -lpthread -I. 

main-highlow: main.cpp highlow-inputvector
	g++ -o $(BUILD_DIR)/main-highlow main.cpp $(BUILD_DIR)/$(HIGHLOW_VEC).o -lpthread -I. 

main-lowhigh: main.cpp lowhigh-inputvector
	g++ -o $(BUILD_DIR)/main-lowhigh main.cpp $(BUILD_DIR)/$(LOWHIGH_VEC).o -lpthread -I. 


# "libraries" to get different vectors in input

default-inputvector: $(DEFAULT_VEC).cpp
	g++ -c $(DEFAULT_VEC).cpp -I. -o $(BUILD_DIR)/$(DEFAULT_VEC).o

updown-inputvector: $(UPDOWN_VEC).cpp
	g++ -c $(UPDOWN_VEC).cpp -I. -o $(BUILD_DIR)/$(UPDOWN_VEC).o

halfdefault-inputvector: $(HALFDEFAULT_VEC).cpp
	g++ -c $(HALFDEFAULT_VEC).cpp -I. -o $(BUILD_DIR)/$(HALFDEFAULT_VEC).o

constant-inputvector: $(CONSTANT_VEC).cpp
	g++ -c $(CONSTANT_VEC).cpp -I. -o $(BUILD_DIR)/$(CONSTANT_VEC).o

highlow-inputvector: $(HIGHLOW_VEC).cpp
	g++ -c $(HIGHLOW_VEC).cpp -I. -o $(BUILD_DIR)/$(HIGHLOW_VEC).o

lowhigh-inputvector: $(LOWHIGH_VEC).cpp
	g++ -c $(LOWHIGH_VEC).cpp -I. -o $(BUILD_DIR)/$(LOWHIGH_VEC).o




all: main-default main-updown main-halfdefault main-constant main-highlow main-lowhigh

clean: clean-o 
	rm $(BUILD_DIR)/main-default $(BUILD_DIR)/main-updown $(BUILD_DIR)/main-halfdefault	\
		$(BUILD_DIR)/main-constant $(BUILD_DIR)/main-highlow $(BUILD_DIR)/main-lowhigh

clean-o:
	rm $(BUILD_DIR)/$(DEFAULT_VEC).o $(BUILD_DIR)/$(UPDOWN_VEC).o $(BUILD_DIR)/$(HALFDEFAULT_VEC).o	\
		  $(BUILD_DIR)/$(CONSTANT_VEC).o $(BUILD_DIR)/$(HIGHLOW_VEC).o $(BUILD_DIR)/$(LOWHIGH_VEC).o

# fast flow
ffmain: src/ff/ffmain.cpp default-inputvector
	g++ -o ffmain-default src/ff/ffmain.cpp $(BUILD_DIR)/$(DEFAULT_VEC).o -lpthread -I. -I./lib/fastflow
