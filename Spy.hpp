// yarinkash1@gmail.com

#pragma once
#include "Character.hpp"

class Player; // Forward declaration of Player class

class Spy: public Character
{
private:
    void peekAndPreventArrest(Player &Player);
    void chooseAction() override;
    bool alreadyPeeked = false; // Flag to check if the Spy has already peeked this turn(so he cannot peek twice or more in a row)
    
public:
    Spy(Player* p, Game* g); 
    void Action() override;
    string getRoleName() const override { return "Spy"; }
    bool getIsAlreadyPeeked() const { return alreadyPeeked; }
    void setAlreadyPeeked(bool value) { alreadyPeeked = value; }

};