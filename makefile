CXX := g++
CXXFLAGS := -w -g -Wall
INC := -I include
SRC := src
OBJ := obj
TARGET := ./bin/chip8

all:
	$(CXX) $(CXXFLAGS) -c $(SRC)/chip8.cpp -o $(OBJ)/chip8.o
	$(CXX) $(CXXFLAGS) $(SRC)/main.cpp $(OBJ)/*.o $(INC) -o $(TARGET)

clean:
	@echo "Cleaning $(TARGET), $(OBJ)"; $(RM) %.o $(TARGET)

.PHONY: all clean


