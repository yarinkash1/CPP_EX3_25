// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Merchant: public Character
{
private:
    void addCoin();
    void chooseAction() override;
    
public:
    Merchant(Player* p, Game* g); 
    void Action() override;
    void Action(Player* target) override;
    std::string getRoleName() const override { return "Merchant"; }
};