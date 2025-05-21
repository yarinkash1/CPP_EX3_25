// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Judge : public Character
{
private:
    void cancelBribe(Player &Player);
    void coinForTheBankOverSanction(Player &Player);
    void chooseAction() override;
    
public:
    Judge(Player* p, Game* g); 
    void Action(int actionType) override; // Override the pure virtual function from Character
    std::string getRoleName() const override { return "Judge"; }
};