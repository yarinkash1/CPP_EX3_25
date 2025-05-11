#pragma once
#include <string>
#include "Character.hpp"
using namespace std;

class Player
{
private:
    // Player's attributes
    string name;     // Player's name
    bool is_active;  // Flag to indicate if it's the player's turn
    int win_counter; // Number of wins
    int coins;       // Number of coins the player has

    // Player's state
    bool isArrested;                 // Flag to indicate if the player is arrested
    bool isSanctioned;               // Flag to indicate if the player is sanctioned until his next turn
    bool isPeekedAndArrestPrevented; // Flag to indicate if the player is peeked and arrest prevented for his next turn

    Character *role; // Pointer to the role (e.g., Governor, Spy)

public:
    Player(std::string name, Character *role, int coins = 0)
        : name(name), is_active(false), win_counter(0), coins(coins),
          isArrested(false), isSanctioned(false),
          isPeekedAndArrestPrevented(false), role(role) {}

    // Getters
    string getName() const { return name; }
    bool getIsActive() const { return is_active; }
    int getWinCounter() const { return win_counter; }
    int getCoins() const { return coins; }
    bool getIsArrested() const { return isArrested; }
    bool getIsSanctioned() const { return isSanctioned; }
    bool getIsPeekedAndArrestPrevented() const { return isPeekedAndArrestPrevented; }

    // Setters
    void setIsActive(bool active) { is_active = active; }
    void setWinCounter(int counter) { win_counter = counter; }
    void setCoins(int new_coins) { coins = new_coins; }
    void setIsArrested(bool arrested) { isArrested = arrested; }
    void setIsSanctioned(bool sanctioned) { isSanctioned = sanctioned; }
    void setIsPeekedAndArrestPrevented(bool peeked_and_arrest_prevented) { isPeekedAndArrestPrevented = peeked_and_arrest_prevented; }
};