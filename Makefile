CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = console_diablo
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run