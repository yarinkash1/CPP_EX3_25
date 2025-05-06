# yarinkash1@gmail.com

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++2a
TARGET = main_exe
TEST_TARGET = test_exe

all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o

$(TEST_TARGET): coup_tests.o
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) coup_tests.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

test.o: coup_tests.cpp
	$(CXX) $(CXXFLAGS) -c coup_tests.cpp

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

PHONY: all clean valgrind

