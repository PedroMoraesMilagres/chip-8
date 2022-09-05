CXX := g++
CXXFLAGS := -w -g -Wall
INC := -I include
SRC := src
OBJ := obj
LIB := -L/lib -lSDL2
TARGET := ./bin/chip8

all:
	$(CXX) $(CXXFLAGS) -c $(SRC)/chip8.cpp -o $(OBJ)/chip8.o
	$(CXX) $(CXXFLAGS) -c $(SRC)/display.cpp -o $(OBJ)/display.o
	$(CXX) $(CXXFLAGS) main.cpp $(OBJ)/*.o $(INC) $(LIB) -o $(TARGET)

clean:
	@echo "Cleaning $(TARGET), $(OBJ)"; $(RM) %.o $(TARGET)

.PHONY: all clean


