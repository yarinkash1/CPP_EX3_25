#pragma once
#include "Character.hpp"

class Governor : public Character 
{
private:
    void tax() override; // Governor's custom tax implementation
    void cancelTax(Player &Player); // Governor's cancel tax action of other player
public:
    void Action(int actionType) override; // Override the pure virtual function from Character
};