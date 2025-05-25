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
#include <vector>
#include <sstream>

using namespace std;

bool isDevMode = false; // Flag for developer mode

// Helper to create a rectangle background for a button text
sf::RectangleShape createButtonShape(const sf::Text &text)
{
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(text.getGlobalBounds().width + 40, text.getGlobalBounds().height + 20));
    shape.setFillColor(sf::Color(50, 50, 50)); // dark gray
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    shape.setPosition(text.getPosition().x + text.getGlobalBounds().width / 2,
                      text.getPosition().y + text.getGlobalBounds().height / 2);
    return shape;
}

// Struct to hold game setup data
struct GameSetupData
{
    int numPlayers;
    vector<string> playerNames;
    bool isValid;
};

GameSetupData showGameSetupPopup(sf::Font &font)
{
    GameSetupData data;
    data.isValid = false;
    data.numPlayers = 2; // Default value

    sf::RenderWindow popup(sf::VideoMode(600, 400), "Game Setup");

    // UI elements
    sf::Text title("Enter Number of Players (2-6)", font, 24);
    title.setFillColor(sf::Color::White);
    title.setPosition(50, 30);

    sf::Text numPlayersText("Players: 2", font, 20);
    numPlayersText.setFillColor(sf::Color::White);
    numPlayersText.setPosition(50, 80);

    sf::Text instruction("Use UP/DOWN arrows to change, ENTER to confirm", font, 16);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(50, 120);

    sf::Text playerNamesTitle("", font, 18);
    playerNamesTitle.setFillColor(sf::Color::White);
    playerNamesTitle.setPosition(50, 160);

    sf::Text currentInput("", font, 16);
    currentInput.setFillColor(sf::Color::Green);
    currentInput.setPosition(50, 200);

    sf::Text playersList("", font, 14);
    playersList.setFillColor(sf::Color::Cyan);
    playersList.setPosition(50, 230);

    bool settingPlayers = true; // true = setting number, false = setting names
    int currentPlayerIndex = 0;
    string currentPlayerName = "";

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                popup.close();
                return data; // Return invalid data
            }

            if (settingPlayers)
            {
                // Handle number of players input
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Up && data.numPlayers < 6)
                    {
                        data.numPlayers++;
                        numPlayersText.setString("Players: " + to_string(data.numPlayers));
                    }
                    else if (event.key.code == sf::Keyboard::Down && data.numPlayers > 2)
                    {
                        data.numPlayers--;
                        numPlayersText.setString("Players: " + to_string(data.numPlayers));
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        settingPlayers = false;
                        data.playerNames.clear();
                        currentPlayerIndex = 0;
                        playerNamesTitle.setString("Enter name for Player " + to_string(currentPlayerIndex + 1) + ":");
                        currentInput.setString("Current: ");
                    }
                }
            }
            else
            {
                // Handle player names input
                if (event.type == sf::Event::TextEntered)
                {
                    if (event.text.unicode == 8)
                    { // Backspace
                        if (!currentPlayerName.empty())
                        {
                            currentPlayerName.pop_back();
                        }
                    }
                    else if (event.text.unicode == 13)
                    { // Enter
                        if (!currentPlayerName.empty())
                        {
                            data.playerNames.push_back(currentPlayerName);
                            currentPlayerName = "";
                            currentPlayerIndex++;

                            if (currentPlayerIndex >= data.numPlayers)
                            {
                                // All players entered
                                data.isValid = true;
                                popup.close();
                            }
                            else
                            {
                                playerNamesTitle.setString("Enter name for Player " + to_string(currentPlayerIndex + 1) + ":");

                                // Update players list display
                                string listStr = "Players added:\n";
                                for (size_t i = 0; i < data.playerNames.size(); i++)
                                {
                                    listStr += to_string(i + 1) + ". " + data.playerNames[i] + "\n";
                                }
                                playersList.setString(listStr);
                            }
                        }
                    }
                    else if (event.text.unicode >= 32 && event.text.unicode < 127)
                    { // Printable characters
                        currentPlayerName += static_cast<char>(event.text.unicode);
                    }

                    currentInput.setString("Current: " + currentPlayerName);
                }
            }
        }

        popup.clear(sf::Color::Black);
        popup.draw(title);

        if (settingPlayers)
        {
            popup.draw(numPlayersText);
            popup.draw(instruction);
        }
        else
        {
            popup.draw(playerNamesTitle);
            popup.draw(currentInput);
            popup.draw(playersList);
        }

        popup.display();
    }

    return data;
}

int main()
{

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

    // Button shapes (background rectangles)
    sf::RectangleShape startButtonShape = createButtonShape(startButton);
    sf::RectangleShape devButtonShape = createButtonShape(devButton);
    sf::RectangleShape quitButtonShape = createButtonShape(quitButton);

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
                {
                    std::cout << "Start Game clicked\n";

                    // Show popup for game setup
                    GameSetupData setupData = showGameSetupPopup(font);

                    if (setupData.isValid)
                    {
                        try
                        {
                            Game::configure(200);
                            // Use the regular getInstance method (without roles parameter)
                            Game& game = Game::getInstance(setupData.numPlayers, setupData.playerNames);
                            
                            printf(" ---------------------------------------------------------------------------------------------------------------------------- \n");
                            cout << "Game started successfully!" << endl;

                            vector<Player *> active_players = game.active_players();
                            for (Player *player : active_players)
                            {
                                player->printPlayerInfo();
                            }

                            int coins_in_bank = game.getCoinsInBank();
                            cout << "Coins in bank: " << coins_in_bank << endl;
                        }
                        catch (const exception &e)
                        {
                            cout << "Error creating game: " << e.what() << endl;
                        }
                    }
                    else
                    {
                        cout << "Game setup cancelled or invalid." << endl;
                    }

                    printf(" ---------------------------------------------------------------------------------------------------------------------------- \n");
                }

                else if (devButton.getGlobalBounds().contains(mousePos))
                {
                    if (isDevMode == true)
                    {
                        std::cout << "Developer Mode game started\n";
                        
                        // Debug constructor with specific roles
                        vector<string> all_characters = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
                        vector<string> player_names = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank"};
                        
                        Game::configure(60);
                        // Use debug constructor with roles parameter
                        Game *game = Game::getInstance(player_names, 6, all_characters);

                        vector<Player *> activePlayers = game->active_players();
                        for (Player *player : activePlayers)
                        {
                            player->printPlayerInfo();
                        }
                    }
                    else
                    {
                        std::cout << "Developer Mode is disabled - button not clickable\n";
                    }
                }

                else if (quitButton.getGlobalBounds().contains(mousePos))
                    window.close();
            }
        }

        // Hover effects
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        startButtonShape.setFillColor(startButtonShape.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color(50, 50, 50));

        if (isDevMode == true)
        {
            devButton.setFillColor(sf::Color::Green);                                                                                     // red text for developer mode
            devButtonShape.setFillColor(devButtonShape.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color(50, 50, 50)); // same dark gray as others
        }

        else
        {
            devButton.setFillColor(sf::Color(100, 100, 100));   // dim gray text
            devButtonShape.setFillColor(sf::Color(30, 30, 30)); // darker background to show it's disabled
        }

        quitButtonShape.setFillColor(quitButtonShape.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color(50, 50, 50));

        window.clear(sf::Color::Cyan);

        // Draw everything
        window.clear(sf::Color::Cyan);

        window.draw(title);

        window.draw(startButtonShape);
        window.draw(startButton);

        window.draw(devButtonShape);
        window.draw(devButton);

        window.draw(quitButtonShape);
        window.draw(quitButton);

        window.display();
    }

    return 0;
}
