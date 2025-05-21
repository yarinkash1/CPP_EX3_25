#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Spy: public Character
{
private:
    void peekAndPreventArrest(Player &Player);
    
public:
    Spy(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
    std::string getRoleName() const override { return "Spy"; }
};