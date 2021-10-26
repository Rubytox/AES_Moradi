CXX = g++
CXXFLAGS = -g -std=c++2a
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
TARGET = attack

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $^

.PHONY:
clean:
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf $(TARGET).exe