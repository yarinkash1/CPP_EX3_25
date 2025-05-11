#pragma once
#include "Character.hpp"
class Player; // Forward declaration of Player class

class General : public Character
{
public:
    void preventCoup(Player &Player);
    void arrestCoinCompensation();
    void Action(int actionType) override; // Override the pure virtual function from Character
};