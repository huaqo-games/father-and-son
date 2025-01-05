CC = g++
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, bin/%.o, $(SRCS))
HDRS = $(wildcard include/*.h)
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LIBS = lib/libraylib.a
OUTFILE = bin/build_osx
FLAGS = -std=c++17
all: build run

bin/%.o: src/%.cpp $(HDRS)
	$(CC) -c $< -o $@ $(FLAGS) $(INCLUDE_PATHS)

build: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(FRAMEWORKS) $(LIBS) -o $(OUTFILE)

run: 
	./$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY: all build run clean
