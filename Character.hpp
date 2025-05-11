#pragma once

// Forward declaration of Player
class Player;

class Character
{
public:
    virtual ~Character() = default; // Virtual destructor for proper cleanup of derived classes

    // Actions
    void gather();
    virtual void tax();
    void bribe();
    void arrest(Player &player);
    void sanction(Player &player);
    void coup(Player &player);
    virtual void Action(int actionType) = 0; // Pure virtual function for derived classes to implement their special actions
};