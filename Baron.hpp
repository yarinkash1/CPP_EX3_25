#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Baron : public Character
{
private:
    void invest();
    void sanctionCoinCompensation();
    Player *player; // Pointer to the Player class to link the Baron to a Player
    
public:
    // Constructor to link the Baron to a Player
    Baron(Player *player);
    void Action(int actionType) override; // Override the pure virtual function from Character
};