#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Governor : public Character 
{
private:
    void tax() override; // Governor's custom tax implementation
    void cancelTax(Player &Player); // Governor's cancel tax action of other player
    Player *player; // Pointer to the Player class to link the Governor to a Player
    
public:
    Governor(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
};