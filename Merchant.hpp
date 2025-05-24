// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Merchant: public Character
{
private:
    void addCoin(); // Adds a coin to the Merchant's total when he starts a turn with 3 coins.
    void chooseAction() override;
    
public:
    Merchant(Player* p, Game* g); 
    void Action() override; // Override the pure virtual function from Character
    std::string getRoleName() const override { return "Merchant"; }
};