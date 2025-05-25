// yarinkash1@gmail.com

#include <iostream>
#include <SFML/Graphics.hpp> // Include SFML for graphics
#include "Game.hpp"          // Include the header file for the Game class
#include "Player.hpp"        // Include the header file for the Player class
#include "Character.hpp"     // Include the header file for the Character class
#include "Baron.hpp"         // Include the header file for the Baron class
#include "General.hpp"       // Include the header file for the General class
#include "Governor.hpp"      // Include the header file for the Governor class
#include "Judge.hpp"         // Include the header file for the Judge class
#include "Merchant.hpp"      // Include the header file for the Merchant class
#include "Spy.hpp"           // Include the header file for the Spy class

int main()
{

    //    vector<string> all_characters = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    //    vector<string> player_names = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank"};
    //    Game::configure(50); // Set initial coins before creating the game instance
    //    Game* game = Game::getInstance(player_names, 6, all_characters);

    //    vector <Player*> activePlayers = game->active_players();
    //    for (Player* player : activePlayers)
    //    {
    //        player->printPlayerInfo(); // Print each player's information
    //    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Cyan);
        window.display();
    }

    return 0;
}
