// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Baron : public Character
{
private:
    void invest();
    void chooseAction() override;
    
public:
    Baron(Player* p, Game* g); 
    void Action() override;
    void Action(Player* target) override; // Override new Action with target
    std::string getRoleName() const override { return "Baron"; }
};