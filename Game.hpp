#pragma once
using namespace std;
#include "Player.hpp"

#include "Baron.hpp"
#include "Spy.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Governor.hpp"
#include "General.hpp"

#include <vector>

#include <algorithm> // For shuffle
#include <random>    // For mt19937 and random_device
#include <chrono>    // For time functions
#include <thread>    // For sleep_for
#include <iomanip>   // For setw, setfill

class Game
{
private:
    int numPlayers; // Number of players in the game
    int currentPlayerIndex; // Index of the current player to indicate whose turn it is
    vector<Player*> players; // Vector to store players pointers
    int coinsInBank; // Total coins in the bank
    bool isGameOver; // Flag to indicate if the game is over

public:
    Game(); // Constructor to initialize the game
    ~Game(); // Destructor to clean up the game
    void endGame(); // Function to end the game
    void nextTurn(); // Function to move to the next player's turn
    void addPlayer(); // Function to add a player to the game
    void removePlayer(Player* player); // Function to remove a player from the game
    Player* current_player(); // Function to get the current player
    vector<Player*> active_players(); // Function to get all active players
    int getCoinsInBank(); // Function to get the total coins in the bank
    bool isGameFinished(); // Function to check if the game is finished
    void resetGame(); // Function to reset the game
    string winner(); // Function to get the winner of the game
    void changeCoinsInBank(int amount);
    void SkipTurn(); // Function to skip the current player's turn
    
};