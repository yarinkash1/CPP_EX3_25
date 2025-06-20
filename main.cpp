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
#include <vector>            // Include vector for dynamic arrays
#include <sstream>           // Include sstream for string stream operations
#include <utility>           // Include utility for std::pair

using namespace std;

bool isDevMode = true; // Flag for developer mode

/**
 * @brief Creates a button shape based on the text size.
 * This function creates a rectangle shape that can be used as a button in the GUI.
 * 
 * @param text The text to be displayed on the button.
 * @return sf::RectangleShape The rectangle shape for the button.
 * @throws None
 */
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

// Struct to hold game setup data(number of players, names and validity flag for the popup)
struct GameSetupData
{
    int numPlayers;
    vector<string> playerNames;
    bool isValid;
};

/**
 * @brief Displays a popup for game setup to enter the number of players and their names.
 * This function creates a popup window where the user can enter the number of players and their names.
 * 
 * @param font The font to be used for text in the popup.
 * @return GameSetupData Struct containing the number of players, player names, and validity flag.
 * @throws None
 */
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

/**
 * @brief Displays a popup window to show game messages.
 * This function collects all messages from the game and displays them in a popup window.
 * The popup can be closed by clicking anywhere or pressing any key.
 * 
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws None
 */
void showMessagePopup(sf::Font &font)
{
    if (!Game::hasMessages())
        return;

    // Collect all messages
    std::vector<std::string> messages;
    while (Game::hasMessages())
    {
        messages.push_back(Game::getNextMessage());
    }

    sf::RenderWindow popup(sf::VideoMode(500, 200 + messages.size() * 25), "Game Messages");

    // Title
    sf::Text title("Game Messages", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(20, 20);

    // Messages
    std::vector<sf::Text> messageTexts;
    for (size_t i = 0; i < messages.size(); i++)
    {
        sf::Text msgText(messages[i], font, 16);
        msgText.setFillColor(sf::Color::Yellow);
        msgText.setPosition(20, 60 + i * 25);
        messageTexts.push_back(msgText);
    }

    // Close instruction
    sf::Text closeText("Click anywhere or press any key to close", font, 14);
    closeText.setFillColor(sf::Color::Cyan);
    closeText.setPosition(20, 60 + messages.size() * 25 + 20);

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::MouseButtonPressed)
            {
                popup.close();
                return;
            }
        }

        popup.clear(sf::Color::Black);
        popup.draw(title);
        for (auto &msgText : messageTexts)
        {
            popup.draw(msgText);
        }
        popup.draw(closeText);
        popup.display();
    }
}

/**
 * @brief Displays a popup window showing the statistics of a player.
 * This function creates a popup window that displays various statistics of the given player.
 * The statistics include player ID, name, coins, active status, role, turn status, and other flags.
 * 
 * @param player Pointer to the Player object whose statistics are to be displayed.
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws None
 */
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
        {"Role:", player->getRole() ? player->getRole()->getRoleName() : "None"},
        {"Is Turn:", player->getIsTurn() ? "Yes" : "No"},
        {"Is Sanctioned:", player->getIsSanctioned() ? "Yes" : "No"},
        {"Peek & Arrest Prevented:", player->getIsPeekedAndArrestPrevented() ? "Yes" : "No"},
        {"Coup Prevented:", player->getIsCoupPrevented() ? "Yes" : "No"},
        {"Tax Prevented:", player->getIsTaxPrevented() ? "Yes" : "No"},
        {"Bribe Prevented:", player->getIsBribePrevented() ? "Yes" : "No"},
        {"Arrested:", player->getIsArrested() ? "Yes" : "No"}};

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

/**
 * @brief Displays a popup window to select a target player for an action.
 * This function creates a popup window that allows the current player to select a target player
 * for actions like arrest, sanction, coup, etc. It filters valid targets based on the action type.
 * 
 * @param gameInstance Pointer to the Game instance.
 * @param currentPlayer Pointer to the Player who is currently taking the action.
 * @param font The font to be used for text in the popup.
 * @param actionType The type of action being performed (e.g., "arrest", "sanction", "general").
 * @return Player* Pointer to the selected target player, or nullptr if no valid targets are available.
 * @throws None
 */
Player *showTargetPlayerPopup(Game *gameInstance, Player *currentPlayer, sf::Font &font, const std::string &actionType = "")
{
    vector<Player *> activePlayers = gameInstance->active_players();
    vector<Player *> validTargets;

    // Remove current player from targets and filter based on action type
    for (Player *player : activePlayers)
    {
        if (player->getId() != currentPlayer->getId())
        {
            // Special filtering for arrest action
            if (actionType == "arrest")
            {
                // Check if player is not already arrested
                if (!player->getIsArrested())
                {
                    // Check coin requirements based on role
                    bool hasEnoughCoins = false;

                    if (player->getRole()->getRoleName() == "Merchant")
                    {
                        // Merchant needs 2+ coins (pays 2 to bank)
                        hasEnoughCoins = (player->getCoins() >= 2);
                    }
                    else
                    {
                        // All other roles need 1+ coin (pay 1 to arresting player)
                        hasEnoughCoins = (player->getCoins() >= 1);
                    }

                    if (hasEnoughCoins)
                    {
                        validTargets.push_back(player);
                    }
                }
            }
            // Special filtering for General's prevent coup action
            else if (actionType == "general")
            {
                // Only show players who can voluntarily choose coup (less than 10 coins)
                // Players with 10+ coins are forced to coup and can't be prevented
                if (player->getCoins() < 10)
                {
                    validTargets.push_back(player);
                }
            }
            // Special filtering for sanction action
            else if (actionType == "sanction")
            {
                // Check if current player can afford to sanction this target
                bool canAffordSanction = false;

                if (player->getRole()->getRoleName() == "Judge")
                {
                    // Sanctioning a Judge costs 4 coins (3 + 1 extra)
                    canAffordSanction = (currentPlayer->getCoins() >= 4);
                }
                else
                {
                    // Sanctioning other roles costs 3 coins
                    canAffordSanction = (currentPlayer->getCoins() >= 3);
                }

                if (canAffordSanction)
                {
                    validTargets.push_back(player);
                }
            }
            else
            {
                // For all other actions, show all players except current player
                validTargets.push_back(player);
            }
        }
    }

    if (validTargets.empty())
    {
        // Show message if no valid targets
        if (actionType == "arrest")
        {
            Game::addMessage("No players available to arrest (all are already arrested or have 0 coins)!");
        }
        return nullptr; // No valid targets
    }

    sf::RenderWindow popup(sf::VideoMode(400, 300 + validTargets.size() * 50), "Select Target Player");

    // Title - update to show action type
    std::string titleText = "Choose a target player:";
    if (actionType == "arrest")
    {
        titleText = "Choose a player to arrest:";
    }
    else if (actionType == "sanction")
    {
        titleText = "Choose a player to sanction:";
    }
    else if (actionType == "coup")
    {
        titleText = "Choose a player to coup:";
    }

    sf::Text title(titleText, font, 20);
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
        string buttonText = to_string(i + 1) + ". " + validTargets[i]->getName();
        sf::Text text(buttonText, font, 16);
        text.setFillColor(sf::Color::White);
        text.setPosition(35, startY + i * 50 + 10);
        playerTexts.push_back(text);
    }

    // Instructions
    sf::Text instruction("Click a player or press ESC to cancel", font, 14);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(20, startY + validTargets.size() * 50 + 20);

    Player *selectedPlayer = nullptr;

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

/**
 * @brief Displays a popup window to force a coup on a player with more than 10 coins.
 * This function creates a popup window that informs the player that they have too many coins
 * and must perform a coup. It allows them to select a target player for the coup.
 * 
 * @param player Pointer to the Player who is forced to coup.
 * @param gameInstance Pointer to the Game instance.
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws None
 */
void showForcedCoupPopup(Player *player, Game *gameInstance, sf::Font &font)
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

    sf::Text coupText("Choose Target to Coup", font, 16);
    coupText.setFillColor(sf::Color::White);
    coupText.setPosition(115, 165);

    // Instructions
    sf::Text instruction("Click to select target or press ESC to cancel", font, 12);
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
                    popup.close(); // Close this popup first

                    // Let player choose target
                    Player *target = showTargetPlayerPopup(gameInstance, player, font, "coup");
                    if (target != nullptr)
                    {
                        // Execute coup with proper target
                        Character *character = player->getRole();
                        character->coup(target); // Pass the selected target

                        // Show any messages from the coup
                        showMessagePopup(font);
                    }
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

/**
 * @brief Executes the role-specific action for a character.
 * This function checks the role of the character and calls the appropriate action method
 * with the target player as an argument.
 * 
 * @param character Pointer to the Character object performing the action.
 * @param target Pointer to the Player who is the target of the action.
 * @param roleName The name of the role being executed.
 * @return void
 * @throws None
 */
void executeRoleAction(Character *character, Player *target, const std::string &roleName)
{
    if (roleName == "Judge")
    {
        static_cast<Judge *>(character)->Action(target); // Use new Action with target
    }
    else if (roleName == "Spy")
    {
        static_cast<Spy *>(character)->Action(target); // Use new Action with target
    }
    else if (roleName == "Governor")
    {
        static_cast<Governor *>(character)->Action(target); // Use new Action with target
    }
}

/**
 * @brief Displays a popup window for the player to choose their action.
 * This function creates a popup window that allows the player to select an action based on their role.
 * It also checks if the player has enough coins and if the action is allowed based on their status.
 * 
 * @param player Pointer to the Player object who is taking the action.
 * @param gameInstance Pointer to the Game instance.
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws std::runtime_error if the player has no valid actions available.
 */
void showPlayerActionPopup(Player *player, Game *gameInstance, sf::Font &font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 500), "Choose Action - " + player->getName());

    // Title
    sf::Text title(player->getName() + " (" + player->getRole()->getRoleName() + ")", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(20, 20);

    // Coins text
    sf::Text coinsText("Coins: " + to_string(player->getCoins()), font, 16);
    coinsText.setFillColor(sf::Color::Yellow);
    coinsText.setPosition(20, 45);

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
        roleSpecificAction = "No special action"; // Merchant has no special action
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
        // Check if player has enough coins for this action
        bool canAfford = true;
        bool isAllowed = true; // Flag for status-based restrictions

        switch (i + 1) // Actions are numbered 1-8
        {
        case 1:                                     // Gather
            canAfford = true;                       // Free action
            isAllowed = !player->getIsSanctioned(); // Cannot gather if sanctioned
            break;
        case 2:                                                                     // Tax
            canAfford = true;                                                       // Free action
            isAllowed = !player->getIsSanctioned() && !player->getIsTaxPrevented(); // Cannot tax if sanctioned or tax prevented
            break;
        case 3: // Bribe
            canAfford = (player->getCoins() >= 4);
            // isAllowed = !player->getIsBribePrevented(); // Cannot bribe if bribe prevented
            break;
        case 4:               // Arrest
            canAfford = true; // Free action
                              // Check if arrest is prevented by Spy's peek ability
            if (player->getIsPeekedAndArrestPrevented())
            {
                isAllowed = false; // Block the button if prevented by Spy
            }
            else
            {
                // Check if there are any valid targets for arrest
                {
                    vector<Player *> activePlayers = gameInstance->active_players();
                    bool hasValidTargets = false;
                    for (Player *p : activePlayers)
                    {
                        if (p->getId() != player->getId() && !p->getIsArrested() && p->getCoins() > 0)
                        {
                            hasValidTargets = true;
                            break;
                        }
                    }
                    isAllowed = hasValidTargets; // Disable button if no valid targets
                }
            }
            break;
        case 5: // Sanction
            canAfford = (player->getCoins() >= 3);
            isAllowed = true; // No status restrictions for sanction
            break;
        case 6: // Coup
            canAfford = (player->getCoins() >= 7);
            isAllowed = !player->getIsCoupPrevented(); // Cannot coup if coup prevented
            break;
        case 7: // Role-specific action
            // Check role-specific costs and restrictions
            if (roleName == "Baron")
            { // Invest costs 3
                canAfford = (player->getCoins() >= 3);
                isAllowed = true; // No status restrictions for Baron's invest
            }
            else if (roleName == "General")
            { // Prevent Coup costs 5
                canAfford = (player->getCoins() >= 5);
                isAllowed = true; // No status restrictions for General's prevent coup
            }
            else if (roleName == "Spy")
            {
                canAfford = true; // Peek is free
                // Spy can't use peek action if already peeked this turn
                Spy *spy = static_cast<Spy *>(player->getRole());
                isAllowed = !spy->getIsAlreadyPeeked(); //  Disable button if already peeked
            }
            else if (roleName == "Merchant")
            {
                // MAKE MERCHANT'S BUTTON NON-CLICKABLE
                canAfford = false; // Set to false to disable the button
                isAllowed = false; // Set to false to disable the button
            }
            else
            {
                canAfford = true; // Other roles have free actions
                isAllowed = true; // No status restrictions for other role actions
            }
            break;
        case 8:               // Skip Turn
            canAfford = true; // Free action
            isAllowed = true; // Always allowed to skip
            break;
        default:
            canAfford = true;
            isAllowed = true;
            break;
        }

        // Combine both conditions
        bool buttonEnabled = canAfford && isAllowed;

        // Button background
        sf::RectangleShape button;
        button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(25, startY + i * 45);
        button.setOutlineThickness(2);

        // Set button colors based on availability
        if (buttonEnabled)
        {
            button.setFillColor(sf::Color(60, 60, 60)); // Normal dark gray
            button.setOutlineColor(sf::Color::White);
        }
        else
        {
            button.setFillColor(sf::Color(30, 30, 30));       // Darker gray for disabled
            button.setOutlineColor(sf::Color(100, 100, 100)); // Dimmer outline
        }

        actionButtons.push_back(button);

        // Button text
        sf::Text buttonText(actions[i], font, 16);
        buttonText.setFillColor(buttonEnabled ? sf::Color::White : sf::Color(120, 120, 120)); // Dimmer text for disabled
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
                        // Recalculate if button is enabled (same logic as above)
                        bool canAfford = true;
                        bool isAllowed = true;

                        switch (i + 1)
                        {
                        case 1: // Gather
                            canAfford = true;
                            isAllowed = !player->getIsSanctioned();
                            break;
                        case 2: // Tax
                            canAfford = true;
                            isAllowed = !player->getIsSanctioned() && !player->getIsTaxPrevented();
                            break;
                        case 3: // Bribe
                            canAfford = (player->getCoins() >= 4);
                            // isAllowed = !player->getIsBribePrevented();
                            break;
                        case 4: // Arrest
                            canAfford = true;
                            if (player->getIsPeekedAndArrestPrevented())
                            {
                                isAllowed = false;
                            }
                            else
                            {
                                vector<Player *> activePlayers = gameInstance->active_players();
                                bool hasValidTargets = false;
                                for (Player *p : activePlayers)
                                {
                                    if (p->getId() != player->getId() && !p->getIsArrested() && p->getCoins() > 0)
                                    {
                                        hasValidTargets = true;
                                        break;
                                    }
                                }
                                isAllowed = hasValidTargets; // Disable button if no valid targets
                            }
                            break;
                        case 5: // Sanction
                            canAfford = (player->getCoins() >= 3);
                            isAllowed = true;
                            break;
                        case 6: // Coup
                            canAfford = (player->getCoins() >= 7);
                            isAllowed = !player->getIsCoupPrevented();
                            break;
                        case 7: // Role-specific action
                            if (roleName == "Baron")
                            {
                                canAfford = (player->getCoins() >= 3);
                                isAllowed = true;
                            }
                            else if (roleName == "General")
                            {
                                canAfford = (player->getCoins() >= 5);
                                isAllowed = true;
                            }

                            else if (roleName == "Spy")
                            {
                                canAfford = true;
                                Spy *spy = static_cast<Spy *>(player->getRole());
                                isAllowed = !spy->getIsAlreadyPeeked(); // Prevent click if already peeked
                            }
                            else if (roleName == "Merchant")
                            {
                                // PREVENT MERCHANT FROM CLICKING THIS BUTTON
                                canAfford = false;
                                isAllowed = false;
                            }
                            else
                            {
                                canAfford = true;
                                isAllowed = true;
                            }
                            break;
                        case 8: // Skip Turn
                            canAfford = true;
                            isAllowed = true;
                            break;
                        }

                        // Only allow click if both conditions are met
                        if (canAfford && isAllowed)
                        {
                            selectedAction = i + 1;
                            break;
                        }
                        else
                        {
                            // Optional: Show message why action is not available
                            if (!canAfford && roleName != "Merchant") // Don't show message for Merchant
                            {
                                Game::addMessage("Not enough coins for this action!");
                            }
                            else if (!isAllowed && roleName != "Merchant") // Don't show message for Merchant
                            {
                                Game::addMessage("This action is currently prevented!");
                            }
                            // For Merchant, do nothing - button is just disabled visually
                            showMessagePopup(font);
                        }
                    }
                }
            }
        }

        // Handle hover effects
        sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
        for (size_t i = 0; i < actionButtons.size(); i++)
        {
            // Only apply hover effects to enabled buttons
            // Recalculate if button is enabled (simplified version)
            bool canAfford = true;
            bool isAllowed = true;

            switch (i + 1)
            {
            case 1: // Gather
                isAllowed = !player->getIsSanctioned();
                break;
            case 2: // Tax
                isAllowed = !player->getIsSanctioned() && !player->getIsTaxPrevented();
                break;
            case 3: // Bribe
                canAfford = (player->getCoins() >= 4);
                // isAllowed = !player->getIsBribePrevented();
                break;
            // And in the hover effects section:
            case 4: // Arrest
                if (player->getIsPeekedAndArrestPrevented())
                {
                    isAllowed = false;
                }
                else
                {
                    vector<Player *> activePlayers = gameInstance->active_players();
                    bool hasValidTargets = false;
                    for (Player *p : activePlayers)
                    {
                        if (p->getId() != player->getId() && !p->getIsArrested() && p->getCoins() > 0)
                        {
                            hasValidTargets = true;
                            break;
                        }
                    }
                    isAllowed = hasValidTargets;
                }
                break;
            case 5: // Sanction
                canAfford = (player->getCoins() >= 3);
                break;
            case 6: // Coup
                canAfford = (player->getCoins() >= 7);
                isAllowed = !player->getIsCoupPrevented();
                break;
            case 7: // Role-specific action
                if (roleName == "Baron")
                {
                    canAfford = (player->getCoins() >= 3);
                    isAllowed = true;
                }
                else if (roleName == "General")
                {
                    canAfford = (player->getCoins() >= 5);
                    isAllowed = true;
                }
                else if (roleName == "Spy")
                {
                    canAfford = true;
                    Spy *spy = static_cast<Spy *>(player->getRole());
                    isAllowed = !spy->getIsAlreadyPeeked(); // Disable button if already peeked
                }
                else if (roleName == "Merchant")
                {
                    // MERCHANT BUTTON ALWAYS DISABLED
                    canAfford = false;
                    isAllowed = false;
                }
                else
                {
                    canAfford = true;
                    isAllowed = true;
                }
                break;
            }

            bool buttonEnabled = canAfford && isAllowed;

            if (buttonEnabled && actionButtons[i].getGlobalBounds().contains(mousePos))
            {
                actionButtons[i].setFillColor(sf::Color::Yellow);
                actionTexts[i].setFillColor(sf::Color::Black);
            }
            else if (buttonEnabled)
            {
                actionButtons[i].setFillColor(sf::Color(60, 60, 60));
                actionTexts[i].setFillColor(sf::Color::White);
            }
            else // ADD THIS ELSE BLOCK FOR DISABLED BUTTONS
            {
                // Keep disabled appearance - don't change on hover
                actionButtons[i].setFillColor(sf::Color(30, 30, 30));  // Dark gray
                actionTexts[i].setFillColor(sf::Color(120, 120, 120)); // Dimmed text
            }
        }

        popup.clear(sf::Color::Black);

        // Draw all elements
        popup.draw(title);
        popup.draw(coinsText);
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
            showMessagePopup(font); // Show any messages from the action
            break;
        case 2:
            character->tax();
            showMessagePopup(font); // Show any messages from the action
            break;
        case 3: // Bribe
            character->bribe();
            showMessagePopup(font); // Show any messages from the action

            // Check if bribe was successful (player still has turn and wasn't prevented)
            if (!player->getIsBribePrevented() && player->getCoins() >= 0) // Successful bribe
            {
                // Show the popup again for extra turn
                showPlayerActionPopup(player, gameInstance, font);
                return; // Exit current popup
            }
            else
            {
                // Bribe was prevented - player loses coins but no extra turn
                gameInstance->nextTurn(); // Move to next player's turn here
                // Don't call showPlayerActionPopup again - turn ends
            }
            break;
        case 4: // Arrest
        {
            Player *target = showTargetPlayerPopup(gameInstance, player, font, "arrest"); // Add "arrest" parameter
            if (target != nullptr)
            {
                character->arrest(target);
                showMessagePopup(font);
            }
            break;
        }
        case 5: // Sanction
        {
            Player *target = showTargetPlayerPopup(gameInstance, player, font, "sanction"); // Add "sanction" parameter
            if (target != nullptr)
            {
                character->sanction(target);
                showMessagePopup(font);
            }
            break;
        }
        case 6: // Coup
        {
            Player *target = showTargetPlayerPopup(gameInstance, player, font, "coup"); // Add "coup" parameter
            if (target != nullptr)
            {
                character->coup(target);
                showMessagePopup(font);
            }
            break;
        }
        case 7:
            if (roleName == "Baron")
            {
                // Baron's invest action
                static_cast<Baron *>(character)->Action();
                showMessagePopup(font);
                gameInstance->nextTurn();
            }
            else if (roleName == "General")
            {
                // General's prevent coup action - needs target
                Player *target = showTargetPlayerPopup(gameInstance, player, font, "general");
                if (target != nullptr)
                {
                    static_cast<General *>(character)->Action(target);
                    showMessagePopup(font);
                    gameInstance->nextTurn();
                }
            }
            else if (roleName == "Judge")
            {
                Player *target = showTargetPlayerPopup(gameInstance, player, font, "judge"); // Add action type
                if (target != nullptr)
                {
                    static_cast<Judge *>(character)->Action(target);
                }
                showMessagePopup(font);
                gameInstance->nextTurn();
            }
            else if (roleName == "Spy")
            {
                // Check if Spy already peeked this turn
                Spy *spy = static_cast<Spy *>(character);
                if (spy->getIsAlreadyPeeked())
                {
                    Game::addMessage("You have already peeked at a player's coins this turn. Choose a different action.");
                    showMessagePopup(font);
                    // Don't end turn - let player choose another action
                    return; // Stay in the same popup to choose another action
                }
                else
                {
                    Player *target = showTargetPlayerPopup(gameInstance, player, font, "spy"); // Add action type
                    if (target != nullptr)
                    {
                        spy->Action(target);
                        showMessagePopup(font);
                        // Spy gets another turn - DON'T call nextTurn()
                        // The popup will close and reopen for the same player
                        return; // This will close current popup and reopen for same player
                    }
                }
            }
            else if (roleName == "Governor")
            {
                Player *target = showTargetPlayerPopup(gameInstance, player, font, "governor"); // Add action type
                if (target != nullptr)
                {
                    static_cast<Governor *>(character)->Action(target);
                }
                showMessagePopup(font);
                gameInstance->nextTurn();
            }
            break;
        case 8:
        {
            // Skip turn
            Game::addMessage(player->getName() + " skipped their turn.");
            gameInstance->nextTurn(); // Same logic as character classes
            showMessagePopup(font);   // Now there will be a message to show
            break;
        }
        break;
        default:
            throw std::runtime_error("Invalid action selected");
            break;
        }
    }
}

/**
 * @brief Displays a popup window to show the game over message and winner.
 * This function creates a popup window that displays the game over message and the name of the winner.
 * It also provides an option to quit the game or return to the main menu.
 * 
 * @param winnerName The name of the player who won the game.
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws None
 */
void showGameOverPopup(const std::string &winnerName, sf::Font &font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 200), "Game Over!");
    sf::Text title("GAME OVER!", font, 28);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(100, 30);

    sf::Text winnerText("Winner: " + winnerName, font, 20);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setPosition(100, 80);

    sf::RectangleShape quitButton(sf::Vector2f(180, 40));
    quitButton.setPosition(110, 140);
    quitButton.setFillColor(sf::Color(150, 0, 0));

    sf::Text quitText("Quit", font, 20);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(180, 145);

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                popup.close();
                exit(0);
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));
                if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    popup.close();
                    exit(0); // Or set your state to MENU if you want to return to menu
                }
            }
        }
        popup.clear(sf::Color::Black);
        popup.draw(title);
        popup.draw(winnerText);
        popup.draw(quitButton);
        popup.draw(quitText);
        popup.display();
    }
}

/**
 * @brief Displays a popup window to inform the player that it's not their turn.
 * This function creates a popup window that shows a message indicating that the player
 * is trying to take an action when it's not their turn.
 * 
 * @param player Pointer to the Player who attempted to take an action.
 * @param font The font to be used for text in the popup.
 * @return void
 * @throws None
 */
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

/**
 * @brief Main function to run the SFML window and game logic.
 * This function initializes the SFML window, handles game states, and manages user input.
 * It also loads the font and sets up the main menu with buttons for starting the game,
 * entering developer mode, or quitting.
 * 
 * @param None
 * @return int Exit code of the program.
 * @throws None
 */
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

                        // Show popup for game setup
                        GameSetupData setupData = showGameSetupPopup(font);

                        if (setupData.isValid)
                        {
                            try
                            {
                                Game::configure(100);
                                // Use the regular getInstance method (without roles parameter)
                                Game &game = Game::getInstance(setupData.numPlayers, setupData.playerNames);
                                gameInstance = &game; // Store the game instance

                                vector<Player *> active_players = game.active_players();

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
                    }

                    else if (devButton.getGlobalBounds().contains(mousePos))
                    {
                        if (isDevMode == true)
                        {
                            std::cout << "Developer Mode game started\n";

                            // Simple approach - use regular constructor then manually set roles
                            vector<string> player_names = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank"};

                            Game::configure(100);

                            try
                            {
                                // Create game with regular constructor (random roles)
                                Game &game = Game::getInstance(player_names.size(), player_names);
                                gameInstance = &game;

                                // Manually assign specific roles
                                vector<Player *> activePlayers = gameInstance->active_players();
                                vector<string> roles = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};

                                for (size_t i = 0; i < activePlayers.size() && i < roles.size(); i++)
                                {
                                    // Delete old role
                                    delete activePlayers[i]->getRole();

                                    // Create new specific role
                                    Character *newRole = nullptr;
                                    if (roles[i] == "Baron")
                                        newRole = new Baron(activePlayers[i], gameInstance);
                                    else if (roles[i] == "General")
                                        newRole = new General(activePlayers[i], gameInstance);
                                    else if (roles[i] == "Governor")
                                        newRole = new Governor(activePlayers[i], gameInstance);
                                    else if (roles[i] == "Judge")
                                        newRole = new Judge(activePlayers[i], gameInstance);
                                    else if (roles[i] == "Merchant")
                                        newRole = new Merchant(activePlayers[i], gameInstance);
                                    else if (roles[i] == "Spy")
                                        newRole = new Spy(activePlayers[i], gameInstance);

                                    // Assign new role
                                    activePlayers[i]->setRole(newRole);
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
                                // MERCHANT PASSIVE ABILITY - add this check at the start of every turn
                                if (activePlayers[i]->getRole()->getRoleName() == "Merchant")
                                {
                                    static_cast<Merchant *>(activePlayers[i]->getRole())->Action();
                                    showMessagePopup(font); // Show the passive ability message
                                }

                                if (activePlayers[i]->getRole()->getRoleName() == "Spy")
                                {
                                    static_cast<Spy *>(activePlayers[i]->getRole())->setAlreadyPeeked(false);
                                }
                                // Check if player has more than 10 coins
                                if (activePlayers[i]->getCoins() >= 10)
                                {
                                    showForcedCoupPopup(activePlayers[i], gameInstance, font);
                                }
                                else
                                {
                                    showPlayerActionPopup(activePlayers[i], gameInstance, font);

                                    // Check if Spy used peek action and gets another turn
                                    if (activePlayers[i]->getRole()->getRoleName() == "Spy")
                                    {
                                        Spy *spy = static_cast<Spy *>(activePlayers[i]->getRole());
                                        if (spy->getIsAlreadyPeeked())
                                        {
                                            // Spy used peek action and gets another turn
                                            Game::addMessage(activePlayers[i]->getName() + " (Spy) gets another turn!");
                                            showMessagePopup(font);
                                            // Show the action popup again for the same player
                                            showPlayerActionPopup(activePlayers[i], gameInstance, font);
                                        }
                                    }
                                }
                                vector<Player *> currentActivePlayers = gameInstance->active_players();
                                if (currentActivePlayers.size() <= 1)
                                {
                                    try
                                    {
                                        string winnerName = gameInstance->winner();
                                        showGameOverPopup(winnerName, font);
                                        // The popup will exit the program, so no need for further logic
                                    }
                                    catch (const invalid_argument &e)
                                    {
                                        Game::addMessage("Unexpected game state!");
                                    }
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
                                    // MERCHANT PASSIVE ABILITY - add this check at the start of every turn
                                    if (activePlayers[i]->getRole()->getRoleName() == "Merchant")
                                    {
                                        static_cast<Merchant *>(activePlayers[i]->getRole())->Action();
                                        showMessagePopup(font); // Show the passive ability message
                                    }
                                    // Reset Spy's peek flag at start of turn
                                    if (activePlayers[i]->getRole()->getRoleName() == "Spy")
                                    {
                                        static_cast<Spy *>(activePlayers[i]->getRole())->setAlreadyPeeked(false);
                                    }
                                    // Check if player has more than 10 coins
                                    if (activePlayers[i]->getCoins() >= 10)
                                    {
                                        showForcedCoupPopup(activePlayers[i], gameInstance, font);
                                    }
                                    else
                                    {
                                        showPlayerActionPopup(activePlayers[i], gameInstance, font);

                                        // Check if Spy used peek action and gets another turn
                                        if (activePlayers[i]->getRole()->getRoleName() == "Spy")
                                        {
                                            Spy *spy = static_cast<Spy *>(activePlayers[i]->getRole());
                                            if (spy->getIsAlreadyPeeked())
                                            {
                                                // Spy used peek action and gets another turn
                                                Game::addMessage(activePlayers[i]->getName() + " (Spy) gets another turn!");
                                                showMessagePopup(font);
                                                // Show the action popup again for the same player
                                                showPlayerActionPopup(activePlayers[i], gameInstance, font);
                                            }
                                        }
                                    }
                                    vector<Player *> currentActivePlayers = gameInstance->active_players();
                                    if (currentActivePlayers.size() <= 1)
                                    {
                                        try
                                        {
                                            string winnerName = gameInstance->winner();
                                            showGameOverPopup(winnerName, font);
                                            // The popup will exit the program, so no need for further logic
                                        }
                                        catch (const invalid_argument &e)
                                        {
                                            Game::addMessage("Unexpected game state!");
                                        }
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