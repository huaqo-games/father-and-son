CXX = g++
CXXFLAGS = -Wall -std=c++17
INCLUDE_PATH = -I./include -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/Cellar/raylib/5.0/lib
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
TARGET = game
SRC = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(SRC) -o $(TARGET) $(LIB_PATH) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: clean $(TARGET)
	./$(TARGET)
