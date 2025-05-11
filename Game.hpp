#pragma once
using namespace std;
#include "Player.hpp"
#include <vector>
class Game
{
private:
    int numPlayers; // Number of players in the game
    int currentPlayerIndex; // Index of the current player to indicate whose turn it is
    vector<Player*> players; // Vector to store players pointers
    int coinsInBank; // Total coins in the bank
    bool isGameOver; // Flag to indicate if the game is over
public:
    Game(int numPlayers, int coinsInBank); // Constructor to initialize the game with a number of players and coins in the bank
    void startGame(); // Function to start the game
    void endGame(); // Function to end the game
    void nextTurn(); // Function to move to the next player's turn
    void addPlayer(Player* player); // Function to add a player to the game
    void removePlayer(Player* player); // Function to remove a player from the game
    Player* current_player(); // Function to get the current player
    vector<Player*> active_players(); // Function to get all active players
    int getCoinsInBank(); // Function to get the total coins in the bank
    bool isGameFinished(); // Function to check if the game is finished
    void resetGame(); // Function to reset the game
    string winner(); // Function to get the winner of the game
};