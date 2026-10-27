CXX ?= g++
CXXFLAGS ?= -Wall -Wextra -g
GLFW_CFLAGS := $(shell pkg-config --cflags glfw3)
GLFW_LIBS := $(shell pkg-config --libs glfw3)

TARGET := glfw_window
SOURCE := main.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) $(SOURCE) -o $(TARGET) $(GLFW_LIBS)

clean:
	rm -f $(TARGET)
