# Makefile for VideoPlayer

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = videoplayer
SRC = VideoPlayer.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
