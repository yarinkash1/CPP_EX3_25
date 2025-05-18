#pragma once
#include <string>
#include "Character.hpp"
using namespace std;

class Player
{
protected:
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
    // Inline constructor with initializer list
    Player(string name, Character *role, int coins = 0, bool is_active = true, int win_counter = 0,
           bool isArrested = false, bool isSanctioned = false, bool isPeekedAndArrestPrevented = false)
        : name(name),
          is_active(is_active),
          win_counter(win_counter),
          coins(coins),
          isArrested(isArrested),
          isSanctioned(isSanctioned),
          isPeekedAndArrestPrevented(isPeekedAndArrestPrevented),
          role(role)
    {
    }

    ~Player()
    {
        delete role;
    }

    // Getters
    string getName() const { return name; }
    bool getIsActive() const { return is_active; }
    int getWinCounter() const { return win_counter; }
    int getCoins() const { return coins; }
    bool getIsArrested() const { return isArrested; }
    bool getIsSanctioned() const { return isSanctioned; }
    bool getIsPeekedAndArrestPrevented() const { return isPeekedAndArrestPrevented; }
    Character *getRole() const { return role; }

    // Setters
    void setIsActive(bool active) { is_active = active; }
    void setWinCounter(int counter) { win_counter = counter; }
    void setCoins(int new_coins) { coins = new_coins; }
    void addNumCoins(int num_coins) { coins += num_coins; }
    void removeNumCoins(int num_coins);
    void setIsArrested(bool arrested) { isArrested = arrested; }
    void setIsSanctioned(bool sanctioned) { isSanctioned = sanctioned; }
    void setIsPeekedAndArrestPrevented(bool peeked_and_arrest_prevented) { isPeekedAndArrestPrevented = peeked_and_arrest_prevented; }
    void setRole(Character *new_role) { role = new_role; }
};