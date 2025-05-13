#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Judge : public Character
{
private:
    void cancelBribe(Player &Player);
    void coinForTheBankOverSanction(Player &Player);
    Player *player; // Pointer to the Player class to link the Judge to a Player
    
public:
    // Constructor to link the Judge to a Player
    Judge(Player *player);
    void Action(int actionType) override; // Override the pure virtual function from Character
};