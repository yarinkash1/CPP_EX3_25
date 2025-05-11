#pragma once
#include "Character.hpp"

class Governor : public Character 
{
public:
    void tax() override; // Governor's custom tax implementation
    void cancelTax(Player &Player); // Governor's cancel tax action of other player
    void Action(int actionType) override; // Override the pure virtual function from Character
};