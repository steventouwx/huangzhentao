SHELL = /bin/sh

# define soucre code directory and header file directory
SRC_DIR := ./interface_cpp/vendor/mediatek/hardware/mtkradioex
SRC_DIR += ./interface_cpp/android/hardware/radio

# auto find all source files (.cpp)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
CPPFLAGS += -I./interface_cpp/include

# define shared libraries dependencies, assume they are in /usr/local/lib directory
LDLIBS := -lbinder -lutils

TARGET := libvendor.mediatek.hardware.mtkradio.so

# define linking options, -shared is used to generate shared library
LDFLAGS := -shared -std=gnu++17 -fPIC -Wall -fno-rtti


# default target
all: $(TARGET)


# build shared library
$(TARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# clean up compiled files
clean:
	rm -f $(TARGET)