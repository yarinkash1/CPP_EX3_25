// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class General : public Character
{
private:
    void preventCoup(Player &Player);
    void chooseAction() override;
    
public:
    General(Player* p, Game* g); 
    void Action() override;
    std::string getRoleName() const override { return "General"; }
};