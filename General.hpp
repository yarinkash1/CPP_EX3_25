#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class General : public Character
{
private:
    void preventCoup(Player &Player);
    void arrestCoinCompensation();
    Player *player; // Pointer to the Player class to link the General to a Player
    
public:
    // Constructor to link the General to a Player
    General(Player* p);  // Declaration only
    void Action(int actionType) override; // Override the pure virtual function from Character
};