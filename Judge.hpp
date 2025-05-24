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
    std::string getRoleName() const override { return "Judge"; }
};