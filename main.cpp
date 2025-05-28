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
#include <utility>

using namespace std;

bool isDevMode = true; // Flag for developer mode

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

// Function to show forced coup popup
void showForcedCoupPopup(Player* player, Game* gameInstance, sf::Font& font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 300), "Forced Coup - " + player->getName());
    
    // Title
    sf::Text title("Too Many Coins!", font, 24);
    title.setFillColor(sf::Color::Red);
    title.setPosition(100, 30);
    
    // Message
    sf::Text message(player->getName() + " has " + to_string(player->getCoins()) + " coins.", font, 16);
    message.setFillColor(sf::Color::White);
    message.setPosition(50, 80);
    
    sf::Text forceMessage("Players with more than 10 coins must coup!", font, 14);
    forceMessage.setFillColor(sf::Color::Yellow);
    forceMessage.setPosition(50, 110);
    
    // Coup button
    sf::RectangleShape coupButton;
    coupButton.setSize(sf::Vector2f(200, 40));
    coupButton.setPosition(100, 150);
    coupButton.setFillColor(sf::Color(150, 50, 50));
    coupButton.setOutlineThickness(2);
    coupButton.setOutlineColor(sf::Color::White);
    
    sf::Text coupText("Coup (7 coins)", font, 16);
    coupText.setFillColor(sf::Color::White);
    coupText.setPosition(140, 165);
    
    // Instructions
    sf::Text instruction("Click to coup or press ESC to cancel", font, 12);
    instruction.setFillColor(sf::Color::Cyan);
    instruction.setPosition(50, 220);
    
    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                popup.close();
                return;
            }
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
                
                if (coupButton.getGlobalBounds().contains(mousePos))
                {
                    // Execute coup
                    Character* character = player->getRole();
                    character->coup(nullptr); // Call the version with parameter, passing nullptr
                    
                    // Use the gameInstance to progress the turn after coup
                    gameInstance->nextTurn();
                    
                    cout << player->getName() << " was forced to coup due to having more than 10 coins." << endl;
                    popup.close();
                    return;
                }
            }
        }
        
        // Handle hover effect
        sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
        if (coupButton.getGlobalBounds().contains(mousePos))
        {
            coupButton.setFillColor(sf::Color::Yellow);
            coupText.setFillColor(sf::Color::Black);
        }
        else
        {
            coupButton.setFillColor(sf::Color(150, 50, 50));
            coupText.setFillColor(sf::Color::White);
        }
        
        popup.clear(sf::Color::Black);
        
        // Draw all elements
        popup.draw(title);
        popup.draw(message);
        popup.draw(forceMessage);
        popup.draw(coupButton);
        popup.draw(coupText);
        popup.draw(instruction);
        
        popup.display();
    }
}



// Function to show player stats in a popup window
void showPlayerStatsPopup(Player *player, sf::Font &font)
{
    sf::RenderWindow popup(sf::VideoMode(500, 700), "Player Stats - " + player->getName());

    // Title
    sf::Text title("Player Statistics", font, 24);
    title.setFillColor(sf::Color::White);
    title.setPosition(150, 20);

    // Create table data using ALL the getter methods from Player.hpp
    vector<pair<string, string>> stats = {
        {"Player ID:", to_string(player->getId())},
        {"Name:", player->getName()},
        {"Coins:", to_string(player->getCoins())},
        {"Is Active:", player->getIsActive() ? "Yes" : "No"},
        {"Win Counter:", to_string(player->getWinCounter())},
        {"Role:", player->getRole() ? player->getRole()->getRoleName() : "None"},
        {"Is Turn:", player->getIsTurn() ? "Yes" : "No"},
        {"Is Sanctioned:", player->getIsSanctioned() ? "Yes" : "No"},
        {"Peek & Arrest Prevented:", player->getIsPeekedAndArrestPrevented() ? "Yes" : "No"},
        {"Coup Prevented:", player->getIsCoupPrevented() ? "Yes" : "No"},
        {"Tax Prevented:", player->getIsTaxPrevented() ? "Yes" : "No"},
        {"Bribe Prevented:", player->getIsBribePrevented() ? "Yes" : "No"},
        {"Arrest Prevented:", player->getIsArrestPrevented() ? "Yes" : "No"}};

    // Create table headers background
    sf::RectangleShape headerBg;
    headerBg.setSize(sf::Vector2f(460, 30));
    headerBg.setPosition(20, 60);
    headerBg.setFillColor(sf::Color(70, 70, 70));
    headerBg.setOutlineThickness(1);
    headerBg.setOutlineColor(sf::Color::White);

    sf::Text headerLabel("Attribute", font, 16);
    headerLabel.setFillColor(sf::Color::White);
    headerLabel.setPosition(30, 68);

    sf::Text headerValue("Value", font, 16);
    headerValue.setFillColor(sf::Color::White);
    headerValue.setPosition(300, 68);

    // Close instruction
    sf::Text closeText("Press ESC or click X to close", font, 14);
    closeText.setFillColor(sf::Color::Yellow);
    closeText.setPosition(20, 650);

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                popup.close();
            }
        }

        popup.clear(sf::Color::Black);

        // Draw title
        popup.draw(title);

        // Draw table header
        popup.draw(headerBg);
        popup.draw(headerLabel);
        popup.draw(headerValue);

        // Draw table rows
        for (size_t i = 0; i < stats.size(); i++)
        {
            float rowY = 100 + i * 35;

            // Alternate row colors
            sf::RectangleShape rowBg;
            rowBg.setSize(sf::Vector2f(460, 35));
            rowBg.setPosition(20, rowY);
            rowBg.setFillColor(i % 2 == 0 ? sf::Color(40, 40, 40) : sf::Color(50, 50, 50));
            rowBg.setOutlineThickness(1);
            rowBg.setOutlineColor(sf::Color(100, 100, 100));
            popup.draw(rowBg);

            // Draw attribute name
            sf::Text attrText(stats[i].first, font, 14);
            attrText.setFillColor(sf::Color::White);
            attrText.setPosition(30, rowY + 8);
            popup.draw(attrText);

            // Draw attribute value with color coding
            sf::Text valueText(stats[i].second, font, 14);

            // Color code the boolean values
            if (stats[i].second == "Yes")
            {
                valueText.setFillColor(sf::Color::Green);
            }
            else if (stats[i].second == "No")
            {
                valueText.setFillColor(sf::Color::Red);
            }
            else
            {
                valueText.setFillColor(sf::Color::Cyan);
            }

            valueText.setPosition(300, rowY + 8);
            popup.draw(valueText);
        }

        // Draw close instruction
        popup.draw(closeText);

        popup.display();
    }
}

// Function to show target player selection popup
Player* showTargetPlayerPopup(Game* gameInstance, Player* currentPlayer, sf::Font& font)
{
    vector<Player*> activePlayers = gameInstance->active_players();
    vector<Player*> validTargets;
    
    // Remove current player from targets
    for (Player* player : activePlayers)
    {
        if (player->getId() != currentPlayer->getId())
        {
            validTargets.push_back(player);
        }
    }
    
    if (validTargets.empty())
    {
        return nullptr; // No valid targets
    }
    
    sf::RenderWindow popup(sf::VideoMode(400, 300 + validTargets.size() * 50), "Select Target Player");
    
    // Title
    sf::Text title("Choose a target player:", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(20, 20);
    
    // Create player buttons
    vector<sf::RectangleShape> playerButtons;
    vector<sf::Text> playerTexts;
    
    float buttonWidth = 350;
    float buttonHeight = 40;
    float startY = 70;
    
    for (size_t i = 0; i < validTargets.size(); i++)
    {
        // Button background
        sf::RectangleShape button;
        button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(25, startY + i * 50);
        button.setFillColor(sf::Color(60, 60, 60));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        playerButtons.push_back(button);
        
        // Button text
        string buttonText = to_string(i + 1) + ". " + validTargets[i]->getName() + 
                           " (" + validTargets[i]->getRole()->getRoleName() + ") - " + 
                           to_string(validTargets[i]->getCoins()) + " coins";
        sf::Text text(buttonText, font, 16);
        text.setFillColor(sf::Color::White);
        text.setPosition(35, startY + i * 50 + 10);
        playerTexts.push_back(text);
    }
    
    // Instructions
    sf::Text instruction("Click a player or press ESC to cancel", font, 14);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(20, startY + validTargets.size() * 50 + 20);
    
    Player* selectedPlayer = nullptr;
    
    while (popup.isOpen() && selectedPlayer == nullptr)
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                popup.close();
                return nullptr; // Cancel selection
            }
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
                
                // Check which button was clicked
                for (size_t i = 0; i < playerButtons.size(); i++)
                {
                    if (playerButtons[i].getGlobalBounds().contains(mousePos))
                    {
                        selectedPlayer = validTargets[i];
                        break;
                    }
                }
            }
        }
        
        // Handle hover effects
        sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
        for (size_t i = 0; i < playerButtons.size(); i++)
        {
            if (playerButtons[i].getGlobalBounds().contains(mousePos))
            {
                playerButtons[i].setFillColor(sf::Color::Yellow);
                playerTexts[i].setFillColor(sf::Color::Black);
            }
            else
            {
                playerButtons[i].setFillColor(sf::Color(60, 60, 60));
                playerTexts[i].setFillColor(sf::Color::White);
            }
        }
        
        popup.clear(sf::Color::Black);
        
        // Draw all elements
        popup.draw(title);
        popup.draw(instruction);
        
        for (size_t i = 0; i < playerButtons.size(); i++)
        {
            popup.draw(playerButtons[i]);
            popup.draw(playerTexts[i]);
        }
        
        popup.display();
    }
    
    popup.close();
    return selectedPlayer;
}


// Function to show player action selection popup
void showPlayerActionPopup(Player *player, Game *gameInstance, sf::Font &font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 500), "Choose Action - " + player->getName());

    // Title
    sf::Text title(player->getName() + " (" + player->getRole()->getRoleName() + ")", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(20, 20);

    // Get role-specific actions
    vector<string> actions;
    string roleSpecificAction = "";

    string roleName = player->getRole()->getRoleName();
    if (roleName == "Baron")
    {
        roleSpecificAction = "7. Invest (3 coins)";
    }
    else if (roleName == "General")
    {
        roleSpecificAction = "7. Prevent Coup (5 coins)";
    }
    else if (roleName == "Governor")
    {
        roleSpecificAction = "7. Cancel Tax (free)";
    }
    else if (roleName == "Judge")
    {
        roleSpecificAction = "7. Cancel Bribe (free)";
    }
    else if (roleName == "Merchant")
    {
        roleSpecificAction = "7. Add Coin (free)";
    }
    else if (roleName == "Spy")
    {
        roleSpecificAction = "7. Peek Cards and Prevent Arrest (free)";
    }

    // Common actions for all roles
    actions = {
        "1. Gather (free)",
        "2. Tax (free)",
        "3. Bribe (4 coins)",
        "4. Arrest (free)",
        "5. Sanction (3 coins)",
        "6. Coup (7 coins)",
        roleSpecificAction,
        "8. Skip Turn (free)"};

    // Special case for Governor - they have "Tax(Governor)" instead of regular tax
    if (roleName == "Governor")
    {
        actions[1] = "2. Tax(Governor)";
    }

    // Create action buttons
    vector<sf::RectangleShape> actionButtons;
    vector<sf::Text> actionTexts;

    float buttonWidth = 350;
    float buttonHeight = 35;
    float startY = 70;

    for (size_t i = 0; i < actions.size(); i++)
    {
        // Button background
        sf::RectangleShape button;
        button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(25, startY + i * 45);
        button.setFillColor(sf::Color(60, 60, 60));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        actionButtons.push_back(button);

        // Button text
        sf::Text buttonText(actions[i], font, 16);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(35, startY + i * 45 + 8);
        actionTexts.push_back(buttonText);
    }

    // Instructions
    sf::Text instruction("Click an action or press ESC to cancel", font, 14);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(20, 450);

    int selectedAction = -1;

    while (popup.isOpen() && selectedAction == -1)
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                popup.close();
                return; // Cancel action selection
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));

                // Check which button was clicked
                for (size_t i = 0; i < actionButtons.size(); i++)
                {
                    if (actionButtons[i].getGlobalBounds().contains(mousePos))
                    {
                        selectedAction = i + 1; // Actions are numbered 1-8
                        break;
                    }
                }
            }
        }

        // Handle hover effects
        sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
        for (size_t i = 0; i < actionButtons.size(); i++)
        {
            if (actionButtons[i].getGlobalBounds().contains(mousePos))
            {
                actionButtons[i].setFillColor(sf::Color::Yellow);
                actionTexts[i].setFillColor(sf::Color::Black);
            }
            else
            {
                actionButtons[i].setFillColor(sf::Color(60, 60, 60));
                actionTexts[i].setFillColor(sf::Color::White);
            }
        }

        popup.clear(sf::Color::Black);

        // Draw all elements
        popup.draw(title);
        popup.draw(instruction);

        for (size_t i = 0; i < actionButtons.size(); i++)
        {
            popup.draw(actionButtons[i]);
            popup.draw(actionTexts[i]);
        }

        popup.display();
    }

    // Execute the selected action
    if (selectedAction != -1)
    {
        popup.close();

        // Call the appropriate action based on selection
        Character *character = player->getRole();

        switch (selectedAction)
        {
        case 1:
            character->gather();
            break;
        case 2:
            character->tax();
            break;
        case 3:
            character->bribe();
            break;
        case 4: // Arrest
        {
            Player* target = showTargetPlayerPopup(gameInstance, player, font);
            if (target != nullptr)
            {
                character->arrest(target); // Pass target to method
            }
            break;
        }
        case 5: // Sanction  
        {
            Player* target = showTargetPlayerPopup(gameInstance, player, font);
            if (target != nullptr)
            {
                character->sanction(target); // Pass target to method
            }
            break;
        }
        case 6: // Coup
        {
            Player* target = showTargetPlayerPopup(gameInstance, player, font);
            if (target != nullptr)
            {
                character->coup(target); // Pass target to method
            }
            break;
        }
        case 7:
            character->Action(); // Call role-specific action
            break;
        case 8:
        {
            // Skip turn
            gameInstance->nextTurn(); // Same logic as character classes
            cout << player->getName() << " skipped their turn." << endl;
            break;
        }
        break;
        default:
            cout << "Invalid action selected." << endl;
            break;
        }

        cout << player->getName() << " selected action " << selectedAction << ": " << actions[selectedAction - 1] << endl;
    }
}

// Function to show "Not Your Turn" message popup
void showNotYourTurnPopup(Player *player, sf::Font &font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 200), "Turn Error");

    // Title
    sf::Text title("Not Your Turn!", font, 24);
    title.setFillColor(sf::Color::Red);
    title.setPosition(120, 30);

    // Message
    sf::Text message("It's not " + player->getName() + "'s turn yet.", font, 16);
    message.setFillColor(sf::Color::White);
    message.setPosition(50, 80);

    // Current turn info
    sf::Text currentTurnText("Please wait for your turn.", font, 14);
    currentTurnText.setFillColor(sf::Color::Yellow);
    currentTurnText.setPosition(50, 110);

    // Close instruction
    sf::Text closeText("Click anywhere in this window or press ESC to close", font, 12);
    closeText.setFillColor(sf::Color::Cyan);
    closeText.setPosition(50, 150);

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::MouseButtonPressed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                popup.close();
            }
        }

        popup.clear(sf::Color::Black);

        // Draw all elements
        popup.draw(title);
        popup.draw(message);
        popup.draw(currentTurnText);
        popup.draw(closeText);

        popup.display();
    }
}


int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Game state enum
    enum GameState
    {
        MENU,
        PLAYING,
        DEV_MODE
    };

    GameState currentState = MENU;

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

    // Game objects (will be created when entering game states)
    Game *gameInstance = nullptr;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Handle window close
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle escape key to return to menu from any state
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                currentState = MENU;
                // Properly cleanup singleton before resetting pointer
                if (gameInstance)
                {
                    Game::cleanup();        // Clean up the singleton instance
                    gameInstance = nullptr; // Reset our local pointer
                }
            }

            // Handle input based on current state
            if (currentState == MENU)
            {
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
                                Game::configure(60);
                                // Use the regular getInstance method (without roles parameter)
                                Game &game = Game::getInstance(setupData.numPlayers, setupData.playerNames);
                                gameInstance = &game; // Store the game instance

                                printf(" ---------------------------------------------------------------------------------------------------------------------------- \n");
                                cout << "Game started successfully!" << endl;

                                vector<Player *> active_players = game.active_players();
                                for (Player *player : active_players)
                                {
                                    player->printPlayerInfo();
                                }

                                int coins_in_bank = game.getCoinsInBank();
                                cout << "Coins in bank: " << coins_in_bank << endl;

                                // Switch to playing state
                                currentState = PLAYING;
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

                            // Use the EXACT same pattern as PLAYING mode
                            try
                            {
                                Game &game = Game::getInstance(player_names.size(), player_names); // Same signature as PLAYING
                                gameInstance = &game;                                              // Same assignment pattern as PLAYING

                                // Manually set roles after creation (if needed)
                                vector<Player *> activePlayers = gameInstance->active_players();
                                for (size_t i = 0; i < activePlayers.size() && i < all_characters.size(); i++)
                                {
                                    // You might need to add a method to set roles or handle this differently
                                    // depending on your Game class implementation
                                }

                                for (Player *player : activePlayers)
                                {
                                    player->printPlayerInfo();
                                }

                                // Switch to dev mode state
                                currentState = DEV_MODE;
                            }
                            catch (const exception &e)
                            {
                                cout << "Error creating dev mode game: " << e.what() << endl;
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
            else if (currentState == PLAYING)
            {
                // Handle game input here - BUTTON CLICKS
                if (event.type == sf::Event::MouseButtonPressed && gameInstance)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    vector<Player *> activePlayers = gameInstance->active_players();
                    int numPlayers = activePlayers.size();

                    // Card dimensions and layout (same as rendering) - remove unused variables
                    float cardSpacingX = 250;
                    float cardSpacingY = 180;
                    float startX = 50;
                    float startY = 80;

                    for (int i = 0; i < numPlayers; i++)
                    {
                        float cardX = startX + (i % 3) * cardSpacingX;
                        float cardY = startY + (i / 3) * cardSpacingY;

                        // Play Turn button bounds
                        sf::FloatRect playTurnBounds(cardX + 20, cardY + 50, 180, 30);
                        if (playTurnBounds.contains(mousePos))
                        {
                            cout << "Play Turn clicked for player: " << activePlayers[i]->getName() << endl;
                            if (activePlayers[i]->getIsTurn() == true)
                            {
                                // Check if player has more than 10 coins
                                if (activePlayers[i]->getCoins() >= 10)
                                {
                                    showForcedCoupPopup(activePlayers[i], gameInstance, font);
                                }
                                else
                                {
                                    showPlayerActionPopup(activePlayers[i], gameInstance, font);
                                }
                            }
                            else
                            {
                                showNotYourTurnPopup(activePlayers[i], font);
                            }
                        }

                        // Show Stats button bounds
                        sf::FloatRect showStatsBounds(cardX + 20, cardY + 90, 180, 30);
                        if (showStatsBounds.contains(mousePos))
                        {
                            cout << "Show Stats clicked for player: " << activePlayers[i]->getName() << endl;
                            showPlayerStatsPopup(activePlayers[i], font);
                        }
                    }
                }
            }
            else if (currentState == DEV_MODE)
            {
                // Handle dev mode input here - BUTTON CLICKS
                if (event.type == sf::Event::MouseButtonPressed && gameInstance)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    vector<Player *> activePlayers = gameInstance->active_players();

                    // Card dimensions and layout (same as rendering) - remove unused variables
                    float cardSpacingX = 250;
                    float cardSpacingY = 180;
                    float startX = 50;
                    float startY = 80;

                    for (int i = 0; i < 6; i++)
                    {
                        // Only handle clicks for active players
                        if (i < static_cast<int>(activePlayers.size()))
                        {
                            float cardX = startX + (i % 3) * cardSpacingX;
                            float cardY = startY + (i / 3) * cardSpacingY;

                            // Play Turn button bounds
                            sf::FloatRect playTurnBounds(cardX + 20, cardY + 50, 180, 30);
                            if (playTurnBounds.contains(mousePos))
                            {
                                cout << "Dev Mode - Play Turn clicked for player: " << activePlayers[i]->getName() << endl;
                                if (activePlayers[i]->getIsTurn() == true)
                                {
                                    // Check if player has more than 10 coins
                                    if (activePlayers[i]->getCoins() >= 10)
                                    {
                                        showForcedCoupPopup(activePlayers[i], gameInstance, font);
                                    }
                                    else
                                    {
                                        showPlayerActionPopup(activePlayers[i], gameInstance, font);
                                    }
                                }
                                else
                                {
                                    showNotYourTurnPopup(activePlayers[i], font);
                                }
                            }

                            // Show Stats button bounds
                            sf::FloatRect showStatsBounds(cardX + 20, cardY + 90, 180, 30);
                            if (showStatsBounds.contains(mousePos))
                            {
                                cout << "Dev Mode - Show Stats clicked for player: " << activePlayers[i]->getName() << endl;
                                showPlayerStatsPopup(activePlayers[i], font);
                            }
                        }
                    }
                }
            }
        }

        // Render based on current state
        window.clear(sf::Color::Cyan);

        if (currentState == MENU)
        {

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

            window.draw(title);

            window.draw(startButtonShape);
            window.draw(startButton);

            window.draw(devButtonShape);
            window.draw(devButton);

            window.draw(quitButtonShape);
            window.draw(quitButton);
        }
        else if (currentState == PLAYING)
        {
            // Draw game elements here
            sf::Text gameText("GAME MODE - Press ESC to return to menu", font, 24);
            gameText.setFillColor(sf::Color::White);
            gameText.setPosition(10, 10);
            window.draw(gameText);

            // Draw player cards if game instance exists
            if (gameInstance)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // For hover detection
                vector<Player *> activePlayers = gameInstance->active_players();
                int numPlayers = activePlayers.size();

                // Card dimensions and layout
                float cardWidth = 220;
                float cardHeight = 150;
                float cardSpacingX = 250;
                float cardSpacingY = 180;
                float startX = 50;
                float startY = 80;

                // Draw cards only for actual players
                for (int i = 0; i < numPlayers; i++)
                {
                    // Calculate card position (3 cards per row)
                    float cardX = startX + (i % 3) * cardSpacingX;
                    float cardY = startY + (i / 3) * cardSpacingY;

                    // Create card rectangle
                    sf::RectangleShape cardRect;
                    cardRect.setSize(sf::Vector2f(cardWidth, cardHeight));
                    cardRect.setPosition(cardX, cardY);
                    cardRect.setFillColor(sf::Color(60, 80, 60)); // Green for active players
                    cardRect.setOutlineThickness(2);
                    cardRect.setOutlineColor(sf::Color::White);

                    // Player name text
                    sf::Text playerName;
                    playerName.setFont(font);
                    playerName.setCharacterSize(18);
                    playerName.setFillColor(sf::Color::White);
                    playerName.setString(activePlayers[i]->getName());
                    playerName.setPosition(cardX + 10, cardY + 10);

                    // Play Turn button with hover effect
                    sf::RectangleShape playTurnButton;
                    playTurnButton.setSize(sf::Vector2f(180, 30));
                    playTurnButton.setPosition(cardX + 20, cardY + 50);
                    playTurnButton.setOutlineThickness(1);
                    playTurnButton.setOutlineColor(sf::Color::White);

                    // Check if mouse is hovering over Play Turn button
                    sf::FloatRect playTurnBounds(cardX + 20, cardY + 50, 180, 30);
                    if (playTurnBounds.contains(mousePos))
                        playTurnButton.setFillColor(sf::Color::Yellow); // Hover color
                    else
                        playTurnButton.setFillColor(sf::Color(70, 70, 150)); // Normal blue

                    sf::Text playTurnText;
                    playTurnText.setFont(font);
                    playTurnText.setString("Play Turn");
                    playTurnText.setCharacterSize(14);
                    playTurnText.setFillColor(sf::Color::White);
                    playTurnText.setPosition(cardX + 65, cardY + 58);

                    // Show Stats button with hover effect
                    sf::RectangleShape showStatsButton;
                    showStatsButton.setSize(sf::Vector2f(180, 30));
                    showStatsButton.setPosition(cardX + 20, cardY + 90);
                    showStatsButton.setOutlineThickness(1);
                    showStatsButton.setOutlineColor(sf::Color::White);

                    // Check if mouse is hovering over Show Stats button
                    sf::FloatRect showStatsBounds(cardX + 20, cardY + 90, 180, 30);
                    if (showStatsBounds.contains(mousePos))
                        showStatsButton.setFillColor(sf::Color::Yellow); // Hover color
                    else
                        showStatsButton.setFillColor(sf::Color(150, 70, 70)); // Normal red

                    sf::Text showStatsText;
                    showStatsText.setFont(font);
                    showStatsText.setString("Show Stats");
                    showStatsText.setCharacterSize(14);
                    showStatsText.setFillColor(sf::Color::White);
                    showStatsText.setPosition(cardX + 60, cardY + 98);

                    // Draw all card elements
                    window.draw(cardRect);
                    window.draw(playerName);
                    window.draw(playTurnButton);
                    window.draw(playTurnText);
                    window.draw(showStatsButton);
                    window.draw(showStatsText);
                }
            }
        }
        else if (currentState == DEV_MODE)
        {
            // Draw dev mode elements here
            sf::Text devText("DEV MODE - Press ESC to return to menu", font, 24);
            devText.setFillColor(sf::Color::White);
            devText.setPosition(10, 10);
            window.draw(devText);

            // Draw player cards if game instance exists
            if (gameInstance)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // For hover detection
                vector<Player *> activePlayers = gameInstance->active_players();

                // Card dimensions and layout
                float cardWidth = 220;
                float cardHeight = 150;
                float cardSpacingX = 250;
                float cardSpacingY = 180;
                float startX = 50;
                float startY = 80;

                // Draw 6 player cards (3x2 grid)
                for (int i = 0; i < 6; i++)
                {
                    // Calculate card position (3 cards per row)
                    float cardX = startX + (i % 3) * cardSpacingX;
                    float cardY = startY + (i / 3) * cardSpacingY;

                    // Create card rectangle
                    sf::RectangleShape cardRect;
                    cardRect.setSize(sf::Vector2f(cardWidth, cardHeight));
                    cardRect.setPosition(cardX, cardY);
                    cardRect.setFillColor(sf::Color(40, 40, 40)); // Dark gray background
                    cardRect.setOutlineThickness(2);
                    cardRect.setOutlineColor(sf::Color::White);

                    // Player name text
                    sf::Text playerName;
                    playerName.setFont(font);
                    playerName.setCharacterSize(18);
                    playerName.setFillColor(sf::Color::White);

                    if (i < static_cast<int>(activePlayers.size()))
                    {
                        // Real player
                        playerName.setString(activePlayers[i]->getName());
                        cardRect.setFillColor(sf::Color(60, 80, 60)); // Slightly green for active players
                    }
                    else
                    {
                        // Empty slot
                        playerName.setString("Empty Slot");
                        playerName.setFillColor(sf::Color(150, 150, 150)); // Gray text for empty slots
                    }

                    playerName.setPosition(cardX + 10, cardY + 10);

                    // Draw card and name FIRST
                    window.draw(cardRect);
                    window.draw(playerName);

                    // Only draw buttons for active players AFTER the card
                    if (i < static_cast<int>(activePlayers.size()))
                    {
                        // Play Turn button with hover effect
                        sf::RectangleShape playTurnButton;
                        playTurnButton.setSize(sf::Vector2f(180, 30));
                        playTurnButton.setPosition(cardX + 20, cardY + 50);
                        playTurnButton.setOutlineThickness(1);
                        playTurnButton.setOutlineColor(sf::Color::White);

                        // Check if mouse is hovering over Play Turn button
                        sf::FloatRect playTurnBounds(cardX + 20, cardY + 50, 180, 30);
                        if (playTurnBounds.contains(mousePos))
                            playTurnButton.setFillColor(sf::Color::Yellow); // Hover color
                        else
                            playTurnButton.setFillColor(sf::Color(70, 70, 150)); // Normal blue

                        sf::Text playTurnText;
                        playTurnText.setFont(font);
                        playTurnText.setString("Play Turn");
                        playTurnText.setCharacterSize(14);
                        playTurnText.setFillColor(sf::Color::White);
                        playTurnText.setPosition(cardX + 65, cardY + 58);

                        // Show Stats button with hover effect
                        sf::RectangleShape showStatsButton;
                        showStatsButton.setSize(sf::Vector2f(180, 30));
                        showStatsButton.setPosition(cardX + 20, cardY + 90);
                        showStatsButton.setOutlineThickness(1);
                        showStatsButton.setOutlineColor(sf::Color::White);

                        // Check if mouse is hovering over Show Stats button
                        sf::FloatRect showStatsBounds(cardX + 20, cardY + 90, 180, 30);
                        if (showStatsBounds.contains(mousePos))
                            showStatsButton.setFillColor(sf::Color::Yellow); // Hover color
                        else
                            showStatsButton.setFillColor(sf::Color(150, 70, 70)); // Normal red

                        sf::Text showStatsText;
                        showStatsText.setFont(font);
                        showStatsText.setString("Show Stats");
                        showStatsText.setCharacterSize(14);
                        showStatsText.setFillColor(sf::Color::White);
                        showStatsText.setPosition(cardX + 60, cardY + 98);

                        // Draw buttons AFTER the card
                        window.draw(playTurnButton);
                        window.draw(playTurnText);
                        window.draw(showStatsButton);
                        window.draw(showStatsText);
                    }
                }
            }
        }
        window.display();
    }

    // Clean up at end of main() function
    // Properly cleanup singleton instances
    if (gameInstance)
    {
        Game::cleanup();
        gameInstance = nullptr;
    }

    return 0;
}