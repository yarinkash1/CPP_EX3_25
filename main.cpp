// yarinkash1@gmail.com
#include <iostream>
#include "Game.hpp"
#include "Player.hpp" // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class

int main()
{
    Game game;
    vector<Player*> active_players = game.active_players();
    //print the names of active players
    for (Player* player : active_players)
    {
        cout << "Active player: " << player->getName() << endl;
    }
    int coins_in_bank = game.getCoinsInBank();
    cout << "Coins in bank: \n" << coins_in_bank << endl;


    int current_coins_player_1 = active_players[0]->getCoins();
    cout << "Current coins of player 1 before gathering: \n" << current_coins_player_1 << endl;


        active_players[0]->getRole()->gather();
        current_coins_player_1 = active_players[0]->getCoins();
        cout << "Current coins of player 1 after gathering: \n" << current_coins_player_1 << endl;
    
    
    return 0;
}
