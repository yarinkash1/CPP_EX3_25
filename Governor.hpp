// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Governor : public Character 
{
private:
    void tax() override; // Governor's custom tax implementation
    void cancelTax(Player &Player); // Governor's cancel tax action of other player
    void chooseAction() override;
    
public:
    Governor(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
    std::string getRoleName() const override { return "Governor"; }
};