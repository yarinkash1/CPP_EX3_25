#pragma once
#include <iostream>

class Player; // Forward declaration of Player class
class Game; // Forward declaration of Game class

class Character
{
protected:
    Player* owner; // Pointer to the owner of the character
    Game* game; // Pointer to the game instance
public:
    Character(Player* p, Game* g); // Constructor to initialize the character with a player and game instance
    virtual ~Character() = default; // Virtual destructor for proper cleanup of derived classes

    // Actions — base versions (can be overridden)
    void gather();
    virtual void tax();
    void bribe();
    void arrest();
    void sanction();
    void coup();

    // Special action handler (for role-specific actions):
    virtual void Action(int actionType) = 0; // Pure virtual function for derived classes to implement their special actions
    Player* target_player(); // Helper function to get the target player
};