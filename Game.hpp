// yarinkash1@gmail.com

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
    string winner_name = ""; // Name of the winner

    // Singleton-related
    Game(int numPlayers, const vector<string>& playerNames); // Private constructor for random roles
    

    ////////////////////////////////////////////////////////////////////////////////
    static Game* instance;

    ///////////////////////////////////////////////////////////////////////////////


    // Private constructor for debugging purposes
    Game(const std::vector<std::string>& playerNames,int numPlayers,const std::vector<std::string>& roles);

    static int initialCoins; // Static variable for configuring initial coins
    Game(const Game&) = delete; // Prevent copying
    Game& operator=(const Game&) = delete; // Prevent assignment

public:
    ~Game(); // Destructor to clean up the game

    static void configure(int coins); // Set initial coins before instance is created

    // Singleton access method
    static Game& getInstance();

    // Regular game creation
    static Game& getInstance(int numPlayers, const vector<string>& playerNames);
        
    // Overloaded getInstance for creating a game with specific players and roles(debugging purposes)
    static Game* getInstance(const std::vector<std::string>& playerNames,int numPlayers,const std::vector<std::string>& roles);

    void endGame(); // Function to end the game
    void resetGame(); // Function to reset the game
    bool isGameFinished(); // Function to check if the game is finished
    string winner(); // Function to get the winner of the game

    void addPlayer(); // Function to add a player to the game
    void removePlayer(Player* player); // Function to remove a player from the game
    Player* current_player(); // Function to get the current player
    vector<Player*> active_players(); // Function to get all active players

    int getCoinsInBank(); // Function to get the total coins in the bank
    void changeCoinsInBank(int amount);

    void nextTurn(); // Function to move to the next player's turn

    void resetPlayerStatus(Player* currentPlayer); // Function to reset player status at the end of their turn
    void addPlayerWithName(const string& playerName);
    static void cleanup(); // Static method to clean up singleton
};