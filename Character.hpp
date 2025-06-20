// yarinkash1@gmail.com

#pragma once
#include <iostream>

class Player; // Forward declaration of Player class
class Game;   // Forward declaration of Game class

class Character
{
    // These are non-owning pointers - Character doesn't create
    // or delete these objects, so no custom cleanup is needed:
protected:
    Player *owner; // Pointer to the owner of the character(Player instance, non-owning pointer - Character doesn't own the Player instance)
    Game *game;    // Pointer to the game instance (non owning pointer - Character doesn't own the Game instance)
public:
    Character(Player *p, Game *g);  // Constructor to initialize the character with a player and game instance
    virtual ~Character() = default; // Virtual destructor for proper cleanup of derived classes

    void gather();
    virtual void tax();
    void bribe();
    virtual void chooseAction() = 0; // Function to choose an action (to be implemented in derived classes)

    // Special action handler (for role-specific actions):
    virtual void Action() = 0; // Pure virtual function for derived classes to implement their special actions
    // Virtual Action that accepts target (default calls Action())
    virtual void Action(Player *target) = 0;

    Player *target_player(); // Helper function to get the target player
    virtual std::string getRoleName() const = 0;

    // Overloaded methods
    void arrest(Player *target = nullptr);
    void sanction(Player *target = nullptr);
    void coup(Player *target = nullptr);
    friend void executeRoleAction(Character *character, Player *target, const std::string &roleName);
};