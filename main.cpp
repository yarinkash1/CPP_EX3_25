// yarinkash1@gmail.com
#include <iostream>
#include "Game.hpp"
#include "Player.hpp" // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class

int main()
{
    Game game(2, 10); // Create a game with 2 players and 10 coins in the bank

    Player player1("Player 1", nullptr, 10);
    Baron baron(&player1);
    
    baron.Action(1); // Call the invest action

    return 0;
}
