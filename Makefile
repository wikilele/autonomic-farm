BUILD_DIR = build
INPUTVECTOR_DIR = lib/inputvectors
DEFAULT_VEC = $(INPUTVECTOR_DIR)/defaultvector
UPDOWN_VEC = $(INPUTVECTOR_DIR)/updownvector
HALFDEFAULT_VEC = $(INPUTVECTOR_DIR)/halfdefaultvector
CONSTANT_VEC = $(INPUTVECTOR_DIR)/constantvector
HIGHLOW_VEC = $(INPUTVECTOR_DIR)/highlowvector
LOWHIGH_VEC = $(INPUTVECTOR_DIR)/lowhighvector

# to allow debug option:
# make main-default DEBUG=1
DEBUG_FLAG = 

ifdef DEBUG
	DEBUG_FLAG = -D DEBUG
endif

# each main-* file gets a different vector in input

main-default: main.cpp default-inputvector
	g++ -o $(BUILD_DIR)/main-default main.cpp $(BUILD_DIR)/$(DEFAULT_VEC).o -lpthread -I. $(DEBUG_FLAG)

main-updown: main.cpp updown-inputvector
	g++ -o $(BUILD_DIR)/main-updown main.cpp $(BUILD_DIR)/$(UPDOWN_VEC).o -lpthread -I. $(DEBUG_FLAG)

main-halfdefault: main.cpp halfdefault-inputvector
	g++ -o $(BUILD_DIR)/main-halfdefault main.cpp $(BUILD_DIR)/$(HALFDEFAULT_VEC).o -lpthread -I. $(DEBUG_FLAG)

main-constant:  main.cpp constant-inputvector
	g++ -o $(BUILD_DIR)/main-constant main.cpp $(BUILD_DIR)/$(CONSTANT_VEC).o -lpthread -I. $(DEBUG_FLAG)

main-highlow: main.cpp highlow-inputvector
	g++ -o $(BUILD_DIR)/main-highlow main.cpp $(BUILD_DIR)/$(HIGHLOW_VEC).o -lpthread -I. $(DEBUG_FLAG)

main-lowhigh: main.cpp lowhigh-inputvector
	g++ -o $(BUILD_DIR)/main-lowhigh main.cpp $(BUILD_DIR)/$(LOWHIGH_VEC).o -lpthread -I. $(DEBUG_FLAG)


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

clean: clean-o clean-ff
	rm $(BUILD_DIR)/main-default $(BUILD_DIR)/main-updown $(BUILD_DIR)/main-halfdefault	\
		$(BUILD_DIR)/main-constant $(BUILD_DIR)/main-highlow $(BUILD_DIR)/main-lowhigh

clean-o:
	rm $(BUILD_DIR)/$(DEFAULT_VEC).o $(BUILD_DIR)/$(UPDOWN_VEC).o $(BUILD_DIR)/$(HALFDEFAULT_VEC).o	\
		  $(BUILD_DIR)/$(CONSTANT_VEC).o $(BUILD_DIR)/$(HIGHLOW_VEC).o $(BUILD_DIR)/$(LOWHIGH_VEC).o


### fast flow
FFFLAGS = -I./lib/fastflow -DUSE_FASTFLOW


ffmain-default: ffmain.cpp default-inputvector
	g++ -o $(BUILD_DIR)/ffmain-default ffmain.cpp $(BUILD_DIR)/$(DEFAULT_VEC).o -lpthread -I. $(FFFLAGS) $(DEBUG_FLAG)

ffmain-constant:  ffmain.cpp constant-inputvector
	g++ -o $(BUILD_DIR)/ffmain-constant main.cpp $(BUILD_DIR)/$(CONSTANT_VEC).o -lpthread -I. $(FFFLAGS) $(DEBUG_FLAG)

all-ff: ffmain-default ffmain-constant

clean-ff:
	rm $(BUILD_DIR)/ffmain-default $(BUILD_DIR)/ffmain-constant