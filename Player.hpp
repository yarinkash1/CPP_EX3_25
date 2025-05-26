// yarinkash1@gmail.com

#pragma once
#include <string>
#include "Character.hpp"
using namespace std;

class Player
{
private:
    static int next_id; // static member to track next ID

protected:
    // Player's attributes
    int id;          // Player's ID
    string name;     // Player's name
    bool is_active;  // Flag to indicate if it's the player's turn
    int win_counter; // Number of wins
    int coins;       // Number of coins the player has
    bool is_turn;    // Flag to indicate if it's the player's turn

    // Player's state
    bool isSanctioned;               // Flag to indicate if the player is sanctioned until his next turn
    bool isPeekedAndArrestPrevented; // Flag to indicate if the player is peeked and arrest prevented for his next turn
    bool isCoupPrevented;            // Flag to indicate if the player is coup prevented
    bool isTaxPrevented;             // Flag to indicate if the player is tax prevented
    bool isBribePrevented;           // Flag to indicate if the player is bribe prevented
    bool isArrestPrevented;          // Flag to indicate if the player is arrest prevented for their next turn

    Character *role; // Pointer to the role (e.g., Governor, Spy)

public:
    // Inline constructor with initializer list
    Player(string name, Character *role, int coins = 0, bool is_active = true, int win_counter = 0,
           bool isSanctioned = false, bool isPeekedAndArrestPrevented = false,
           bool isCoupPrevented = false, bool isTaxPrevented = false, bool isBribePrevented = false, bool isArrestPrevented = false)
        : id(next_id++), // Increment ID for each new player
          name(name),
          is_active(is_active),
          win_counter(win_counter),
          coins(coins),
          is_turn(false),
          isSanctioned(isSanctioned),
          isPeekedAndArrestPrevented(isPeekedAndArrestPrevented),
          isCoupPrevented(isCoupPrevented),
          isTaxPrevented(isTaxPrevented),
          isBribePrevented(isBribePrevented),
          isArrestPrevented(isArrestPrevented),
          role(role)
    {
    }

    ~Player()
    {
        delete role;
    }

    void printPlayerInfo();

    // Forward declaration to avoid dependency on sf::Font in header
    class Font; // Forward declare Font if possible, or include <SFML/Graphics/Font.hpp> if needed
    friend void showPlayerStatsPopup(Player *player, Font &font);

    // Getters
    string getName() const { return name; }
    bool getIsActive() const { return is_active; }
    int getWinCounter() const { return win_counter; }
    int getCoins() const { return coins; }

    bool getIsSanctioned() const { return isSanctioned; }
    bool getIsPeekedAndArrestPrevented() const { return isPeekedAndArrestPrevented; } //
    bool getIsCoupPrevented() const { return isCoupPrevented; }                       //
    bool getIsTaxPrevented() const { return isTaxPrevented; }                         //
    bool getIsBribePrevented() const { return isBribePrevented; }                     //
    bool getIsArrestPrevented() const { return isArrestPrevented; }

    int getId() const { return id; }
    Character *getRole() const { return role; }
    bool getIsTurn() const { return is_turn; }

    // Setters
    void setIsActive(bool active) { is_active = active; }
    void setWinCounter(int counter) { win_counter = counter; }
    void setCoins(int new_coins) { coins = new_coins; }
    void addNumCoins(int num_coins) { coins += num_coins; }
    void removeNumCoins(int num_coins);
    void setIsSanctioned(bool sanctioned) { isSanctioned = sanctioned; }
    void setIsPeekedAndArrestPrevented(bool peeked_and_arrest_prevented) { isPeekedAndArrestPrevented = peeked_and_arrest_prevented; }
    void setIsCoupPrevented(bool coup_prevented) { isCoupPrevented = coup_prevented; }
    void setIsTaxPrevented(bool tax_prevented) { isTaxPrevented = tax_prevented; }
    void setIsBribePrevented(bool bribe_prevented) { isBribePrevented = bribe_prevented; }
    void setIsArrestPrevented(bool arrest_prevented) { isArrestPrevented = arrest_prevented; }
    static void resetIdCounter() { next_id = 1; }
    void setId(int new_id) { id = new_id; }
    void setRole(Character *new_role) { role = new_role; }
    void setIsTurn(bool turn) { is_turn = turn; }
};