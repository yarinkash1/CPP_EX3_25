#pragma once
#include "Character.hpp"
class Player; // Forward declaration of Player class

class Spy: public Character
{
public:
    void peekAndPreventArrest(Player &Player);
    void Action(int actionType) override; // Override the pure virtual function from Character
};