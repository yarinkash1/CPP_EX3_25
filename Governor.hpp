// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Governor : public Character 
{
private:
    void tax() override;
    void cancelTax(Player &Player);
    void chooseAction() override;
    
public:
    Governor(Player* p, Game* g); 
    void Action() override;
    std::string getRoleName() const override { return "Governor"; }
};