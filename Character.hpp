#pragma once
#include <iostream>

class Player; // Forward declaration of Player class


class Character
{
private:
    Player* owner; // Pointer to the owner of the character
public:
    Character(Player* p) : owner(p) {}  // Set owner in base constructor
    virtual ~Character() = default; // Virtual destructor for proper cleanup of derived classes

    // Actions — base versions (can be overridden)
    void gather();
    virtual void tax();
    void bribe();
    void arrest(Player &player);
    void sanction(Player &player);
    void coup(Player &player);

    // Special action handler (for role-specific actions):
    virtual void Action(int actionType) = 0; // Pure virtual function for derived classes to implement their special actions
};