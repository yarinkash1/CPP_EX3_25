// yarinkash1@gmail.com

#include <iostream>
#include "Game.hpp"
#include "Player.hpp"    // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class
#include "Character.hpp" // Include the header file for the Character class

int main()
{
    // -- Creating a new game:
    // Configure the game with initial coins
    Game::configure(200);           // Set initial coins in bank (This line can be depracted if we want the deafault value of 50)
    Game &g1 = Game::getInstance(); // Create singleton instance

    printf(" ---------------------------------------------------------------------------------------------------------------------------- \n");
    Game &g2 = Game::getInstance(); // Create another game instance to test singleton behavior
    if (&g1 == &g2)
    {
        cout << "Singleton works! Both game instances are the same." << endl;
    }
    else
    {
        cout << "Singleton failed! Different game instances." << endl;
    }

    // Game g3;                    //  Error: constructor is private
    // Game g4 = g1;               //  Error: copy constructor is deleted
    // Game g5;
    // g5 = g1;                    //  Error: assignment operator is deleted

    printf(" ---------------------------------------------------------------------------------------------------------------------------- \n");

    vector<Player *> active_players = g1.active_players(); // Get the list of active players from the game we created

    // Print the stats of the players:
    for (Player *player : active_players)
    {
        player->printPlayerInfo();
    }

    // Print the coins in the bank after the game is created:
    int coins_in_bank = g1.getCoinsInBank();
    cout << "Coins in bank: " << coins_in_bank << endl;

    // -- Gather example:

    // Print the current coins of player 2 before gathering:
    int current_coins_player_2 = active_players[1]->getCoins();
    printf("Current coins of player 2 (ID: %d, %s) before gathering: %d\n", active_players[1]->getId(),
           active_players[1]->getName().c_str(), current_coins_player_2);

    // print the current coins of player 2 after gathering:
    active_players[1]->getRole()->gather();
    current_coins_player_2 = active_players[1]->getCoins();
    printf("Current coins of player 2 (ID: %d, %s) after gathering: %d\n", active_players[1]->getId(),
           active_players[1]->getName().c_str(), current_coins_player_2);

    // Print the coins in the bank after gathering:
    cout << "Coins in bank after gathering: " << g1.getCoinsInBank() << endl;

    //  -- Tax example:
    // Print the current coins of player 1 before taxing:
    int current_coins_player_1 = active_players[0]->getCoins();
    printf("Current coins of player 1 (ID: %d, %s) before taxing: %d\n", active_players[0]->getId(),
           active_players[0]->getName().c_str(), current_coins_player_1);
    // print the current coins of player 1 after taxing:
    active_players[0]->getRole()->tax();
    current_coins_player_1 = active_players[0]->getCoins();
    printf("Current coins of player 1 (ID: %d, %s) after taxing: %d\n", active_players[0]->getId(),
           active_players[0]->getName().c_str(), current_coins_player_1);
    // Print the coins in the bank after taxing:
    cout << "Coins in bank after taxing: " << g1.getCoinsInBank() << endl;
    g1.nextTurn(); // Move to the next player's turn

    // -- Bribe example:
    // Print the current coins of player 1 before bribing:
    current_coins_player_1 = active_players[0]->getCoins();
    printf("Current coins of player 1 (ID: %d, %s) before bribing: %d\n", active_players[0]->getId(),
           active_players[0]->getName().c_str(), current_coins_player_1);
    g1.nextTurn(); // Move to the next player's turn

    active_players[0]->getRole()->chooseAction(); // Call the chooseAction method


    return 0;
}
