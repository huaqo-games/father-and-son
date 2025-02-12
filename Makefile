CXX = clang++
FLAGS = -Wall -std=c++17 $(shell pkg-config --libs --cflags raylib)
TARGET = game
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(FLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: clean $(TARGET)
	./$(TARGET)
