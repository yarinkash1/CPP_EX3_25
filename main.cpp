// yarinkash1@gmail.com

#include <iostream>
#include "Game.hpp"
#include "Player.hpp"    // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class
#include "Character.hpp" // Include the header file for the Character class

int main()
{

       vector<string> all_characters = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
       vector<string> player_names = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank"};
       Game::configure(50); // Set initial coins before creating the game instance
       Game* game = Game::getInstance(player_names, 6, all_characters);

       vector <Player*> activePlayers = game->active_players();
       for (Player* player : activePlayers)
       {
           player->printPlayerInfo(); // Print each player's information
       }

       return 0;
}
