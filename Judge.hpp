// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Judge : public Character
{
private:
    void cancelBribe(Player &Player);
    void chooseAction() override;
    
public:
    Judge(Player* p, Game* g); 
    void Action() override;
    void Action(Player* target) override; // Override new Action with target
    std::string getRoleName() const override { return "Judge"; }
};