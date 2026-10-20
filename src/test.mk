# build the desktop debug version
# make -f test.mk clean; make -f test.mk

CXX ?= g++
RM  ?= rm -f

CXXFLAGS ?= -DDESKTOP_DEBUG -DUSBSERIAL_DEBUG_LEVEL=4 -g -Og -flto -ffunction-sections -fdata-sections \
            -fmessage-length=0 \
            -fno-tree-loop-distribute-patterns
CPPFLAGS += -I. -Wall $(EXTRA_CFLAGS)
LDFLAGS  ?= -flto -Wl,--print-memory-usage -Wl,-Map=test.map -static-libgcc 
LDLIBS += -lncurses 

SRCS := debugmain.cpp driver_api.cpp Driver.cpp \
		tinytapeout/settings.c \
		tinytapeout/Globals.cpp \
		tinytapeout/Command.cpp \
		tinytapeout/CommandInterpreter.cpp \
		tinytapeout/USBSerial.cpp \
		tinytapeout/ProjectMux.cpp \
		platform/Clock.cpp \
		platform/IC.cpp
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

.PHONY: all clean
all: test

test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS) test test.map