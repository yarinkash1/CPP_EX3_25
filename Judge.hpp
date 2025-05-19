#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Judge : public Character
{
private:
    void cancelBribe(Player &Player);
    void coinForTheBankOverSanction(Player &Player);
    
public:
    Judge(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
};