#pragma once
#include "Character.hpp"
class Player; // Forward declaration of Player class

class Merchant: public Character
{
private:
    void addCoin(); // Adds a coin to the Merchant's total when he starts a turn with 3 coins.
    void arrestAlternate(); // If the Merchant is attacked by arrest action he is paying 2 coins to the bank instead of losing 1 coin to other player.
public:
    void Action(int actionType) override; // Override the pure virtual function from Character
};