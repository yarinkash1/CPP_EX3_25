# yarinkash1@gmail.com

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++2a
TARGET = main_exe
TEST_TARGET = test_exe
GUI_TARGET = sfml-app

# Object files that both main and tests need
GAME_OBJECTS = Game.o Player.o Character.o Baron.o General.o Governor.o Judge.o Merchant.o Spy.o

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system


all: $(TARGET) $(TEST_TARGET) $(GUI_TARGET)

Main: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): main.o %$(GAME_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o $(GAME_OBJECTS) $(SFML_LIBS)

$(TEST_TARGET): coup_tests.o $(GAME_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) coup_tests.o $(GAME_OBJECTS) $(SFML_LIBS)

$(GUI_TARGET): main.o $(GAME_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(GUI_TARGET)  main.o $(GAME_OBJECTS) $(SFML_LIBS)

main.o: main.cpp Game.hpp Player.hpp Character.hpp Baron.hpp General.hpp Governor.hpp Judge.hpp Merchant.hpp Spy.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Game.o: Game.cpp Game.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Game.cpp

Player.o: Player.cpp Player.hpp Character.hpp
	$(CXX) $(CXXFLAGS) -c Player.cpp

Character.o: Character.cpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Character.cpp

Baron.o: Baron.cpp Baron.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Baron.cpp

General.o: General.cpp General.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c General.cpp

Governor.o: Governor.cpp Governor.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Governor.cpp

Judge.o: Judge.cpp Judge.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Judge.cpp

Merchant.o: Merchant.cpp Merchant.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Merchant.cpp

Spy.o: Spy.cpp Spy.hpp Character.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Spy.cpp

coup_tests.o: coup_tests.cpp Game.hpp Player.hpp Character.hpp Baron.hpp General.hpp Governor.hpp Judge.hpp Merchant.hpp Spy.hpp
	$(CXX) $(CXXFLAGS) -c coup_tests.cpp

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(GUI_TARGET) *.o

.PHONY: all clean valgrind Main test

