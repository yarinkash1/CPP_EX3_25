#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Spy: public Character
{
private:
    void peekAndPreventArrest(Player &Player);
    Player *player; // Pointer to the Player class to link the Spy to a Player
    
public:
    // Constructor to link the Spy to a Player
    Spy(Player* p);  // Declaration only
    void Action(int actionType) override; // Override the pure virtual function from Character
};