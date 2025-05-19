#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class General : public Character
{
private:
    void preventCoup(Player &Player);
    void arrestCoinCompensation();
    
public:
    General(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
};