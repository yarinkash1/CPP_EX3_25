#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Baron : public Character
{
private:
    void invest();
    void sanctionCoinCompensation();
    
public:
    Baron(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
    std::string getRoleName() const override { return "Baron"; }
};