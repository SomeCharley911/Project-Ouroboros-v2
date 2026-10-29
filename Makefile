# force macs to use g++-15 to avoid clang mapped to g++
ifeq ($(OS),Windows_NT)
	CXX = g++
	OPENGL_LIBS = -lopengl32
else
	OS_NAME := $(shell uname -s)
	ifeq ($(OS_NAME),Darwin)
		CXX = g++-15
		OPENGL_LIBS = -framework OpenGL
	else
		CXX = g++
		OPENGL_LIBS = -lGL
	endif
endif

# compiler options
CXXFLAGS = -g -Wall -Wextra
CXXFLAGS += -std=c++20 -fmodules-ts -pedantic -pedantic-errors
CXXFLAGS += -Wfloat-equal -Wredundant-decls -Wshadow -Wconversion -Wuninitialized

GLFW_CFLAGS = $(shell pkg-config --cflags glfw3)
GLFW_LIBS = $(shell pkg-config --libs glfw3)

# list .cpp files here according to main program, tests, or both
PROGRAM_FILES = main.cpp
TEST_FILES = tests.cpp
SHARED_FILES = Objects/Point.cpp Objects/Triangle.cpp Objects/Plate.cpp

.PHONY: all
all: program.exe tests.exe

.PHONY: program.exe # Always force rebuild of tests
program.exe: $(SHARED_FILES) $(PROGRAM_FILES)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) $(SHARED_FILES) $(PROGRAM_FILES) -o $@ $(GLFW_LIBS) $(OPENGL_LIBS)

.PHONY: tests.exe # Always force rebuild of tests
tests.exe:  $(SHARED_FILES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) $(SHARED_FILES) $(TEST_FILES) -o $@ $(GLFW_LIBS) $(OPENGL_LIBS)

.PHONY: clean
clean:
	rm -f program.exe tests.exe