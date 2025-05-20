// yarinkash1@gmail.com
#include <iostream>
#include "Game.hpp"
#include "Player.hpp"    // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class

int main()
{
    Game game;
    vector<Player *> active_players = game.active_players();
    // print the names of active players
    for (Player *player : active_players)
    {
        cout << "ID: " << player->getId() << " Player name: " << player->getName() << endl;
    }
    int coins_in_bank = game.getCoinsInBank();
    cout << "Coins in bank: " << coins_in_bank << endl;

    int current_coins_player_2 = active_players[1]->getCoins();
    printf("Current coins of player 2 (ID: %d, %s) before gathering: %d\n", active_players[1]->getId(), active_players[1]->getName().c_str(), current_coins_player_2);

    active_players[1]->getRole()->gather();
    current_coins_player_2 = active_players[1]->getCoins();
    printf("Current coins of player 2 (ID: %d, %s) after gathering: %d\n", active_players[1]->getId(), active_players[1]->getName().c_str(),current_coins_player_2);
    
    cout << "Coins in bank after gathering: "<< game.getCoinsInBank() << endl;

    return 0;
}
