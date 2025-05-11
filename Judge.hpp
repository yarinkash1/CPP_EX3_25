#pragma once
#include "Character.hpp"
class Player; // Forward declaration of Player class

class Judge : public Character
{
public:
    void cancelBribe(Player &Player);
    void coinForTheBankOverSanction(Player &Player);
    void Action(int actionType) override; // Override the pure virtual function from Character
};