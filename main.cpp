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

    // Load font
    sf::Font font;
    if (!font.loadFromFile("fonts/OpenSans_Regular.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // Title text
    sf::Text title("COUP GAME", font, 48);
    title.setFillColor(sf::Color::White);
    title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 50);

    
    // Button texts
    sf::Text startButton("Start Game", font, 30);
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(800 / 2 - startButton.getGlobalBounds().width / 2, 200);

    sf::Text devButton("Dev Mode", font, 30);
    devButton.setFillColor(sf::Color::White);
    devButton.setPosition(800 / 2 - devButton.getGlobalBounds().width / 2, 270);

    sf::Text quitButton("Quit", font, 30);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setPosition(800 / 2 - quitButton.getGlobalBounds().width / 2, 340);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Handle window close
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (startButton.getGlobalBounds().contains(mousePos))
                    std::cout << "Start Game clicked\n";

                else if (devButton.getGlobalBounds().contains(mousePos))
                    std::cout << "Dev Mode clicked\n";

                else if (quitButton.getGlobalBounds().contains(mousePos))
                    window.close();
            }
        }

        window.clear(sf::Color::Cyan);

        // Draw UI
        window.draw(title);
        window.draw(startButton);
        window.draw(devButton);
        window.draw(quitButton);

        window.display();
    }

    return 0;
}
