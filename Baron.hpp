#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Baron : public Character
{
private:
    void invest();
    void sanctionCoinCompensation();
public:
    void Action(int actionType) override; // Override the pure virtual function from Character
};