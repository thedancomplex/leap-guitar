OS := $(shell uname)
ARCH := $(shell uname -m)
SYS_NAME=midi_hand

CPPFLAGS += -I/usr/local/include

LDFLAGS += -L/usr/local/lib

LDLIBS += -lrtmidi
LDLIBS += /usr/local/lib/librtmidi.so




$(SYS_NAME): $(SYS_NAME).cpp
	$(CXX) -Wall -g $(SYS_NAME).cpp -o $(SYS_NAME) -I /usr/local/include/rtmidi/ -L $(LDLIBS)

clean:
	rm -rf $(SYS_NAME) $(SYS_NAME).dSYM
