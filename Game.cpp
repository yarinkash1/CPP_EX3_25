// yarinkash1@gmail.com

#include "Game.hpp"

int Game::initialCoins = 50; // Default initial coins (can be changed by configure method)

Game* Game::instance = nullptr;  // Singleton instance pointer

std::queue<std::string> Game::messageQueue; // Queue to hold messages for the game


/**
 * @brief Factory function to create a Character object based on the role.
 * This function takes a role name and returns a new Character object of the corresponding type.
 *
 * @param role The role name as a string.
 * @param player Pointer to the Player object associated with the character.
 * @param game Pointer to the Game object.
 * @return Character* Pointer to the newly created Character object
 * @throws invalid_argument if the role is not recognized.
 */
Character *createCharacterByRole(const string &role, Player *player, Game *game)
{
    if (role == "Baron")
        return new Baron(player, game);
    else if (role == "General")
        return new General(player, game);
    else if (role == "Governor")
        return new Governor(player, game);
    else if (role == "Judge")
        return new Judge(player, game);
    else if (role == "Merchant")
        return new Merchant(player, game);
    else if (role == "Spy")
        return new Spy(player, game);
    else
        throw invalid_argument("Invalid role: " + role);
}


/**
 * @brief Change the number of coins in the bank by a specified amount.
 * This function allows you to add or remove coins from the bank.
 * If the amount is negative, it will remove coins; if positive, it will add coins.
 *
 * @param amount The amount of coins to change in the bank. Can be positive or negative.
 * @throws invalid_argument if the resulting coins in the bank would be negative.
 * @return void
 */
void Game::changeCoinsInBank(int amount)
{
    if (coinsInBank + amount < 0)
    {
        throw invalid_argument("Not enough coins in the bank.");
    }
    coinsInBank += amount;
}

/**
 * @brief Destructor for the Game class.
 * Cleans up the players vector by deleting each Player object.
 *
 * @param none
 * @return void
 * @throws none
 */
Game::~Game()
{
    for (Player *p : players)
    {
        delete p;
    }
    players.clear();
}


/**
 * @brief Adds a player to the game with a specified name.
 * This function creates a Player object with a random role and adds it to the game's player list.
 * Multiple players can have the same role.
 *
 * @param playerName The name of the player to be added.
 * @return void
 * @throws none
 */
void Game::addPlayerWithName(const string& playerName)
{
    Player *player = new Player(playerName, nullptr);

    // Available roles
    vector<string> available_roles = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    
    // Randomly select a role (duplicates allowed)
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> dist(0, available_roles.size() - 1);
    string role = available_roles[dist(g)];
    
    Character *character = createCharacterByRole(role, player, this);
    player->setRole(character);
    players.push_back(player);
    numPlayers++; // Update the player count
}

/**
 * @brief Constructor for the Game class.
 * Initializes the game with a specified number of players(chosen by the user in the GUI)
 * and sets up the game providing each player with a random role.
 *
 * @param numPlayers The number of players in the game.
 * @param playerNames A vector of strings containing the names of the players.
 * @return void
 * @throws invalid_argument if the number of players is not between 2 and 6.
 */
Game::Game(int numPlayers, const vector<string>& playerNames)
{

    if (numPlayers < 2 || numPlayers > 6)
    {
        throw invalid_argument("Number of players must be between 2 and 6.");
    }

    if (playerNames.size() != static_cast<size_t>(numPlayers))
    {
        throw invalid_argument("Number of player names must match number of players.");
    }

    this->numPlayers = 0; // Will be incremented by addPlayerWithName
    this->coinsInBank = initialCoins;
    this->currentPlayerIndex = 0;
    this->isGameOver = false;
    
    // Use the provided player names instead of asking for input
    for (const string& playerName : playerNames)
    {
        addPlayerWithName(playerName); // Use the names from the popup
    }

    // Set the first player as has turn:
    if (!players.empty()) 
    {
        players[0]->setIsTurn(true);
    }
}

/**
 * @brief Get a vector of active players in the game.
 * This function iterates through the players vector and returns a vector containing pointers to all active players (those with is_active set to true).
 *
 * @param none
 * @return vector<Player*> A vector of pointers to active Player objects.
 * @throws none
 */
vector<Player *> Game::active_players()
{
    vector<Player *> activePlayers;
    for (Player *player : players)
    {
        if (player->getIsActive() == 1) // 1 for true
        {
            activePlayers.push_back(player);
        }
    }
    return activePlayers;
}

/**
 * @brief Get the total number of coins in the bank.
 * This function returns the current number of coins in the bank.
 *
 * @param none
 * @return int The total number of coins in the bank.
 * @throws none
 */
int Game::getCoinsInBank()
{
    return coinsInBank;
}

void Game::resetPlayerStatus(Player *currentPlayer)
{
    // Reset the Spy's peeked status if the current player is a Spy
    Spy *spyRole = dynamic_cast<Spy *>(currentPlayer->getRole());
    if (spyRole != nullptr)
    {
        spyRole->setAlreadyPeeked(false); // Reset the Spy's peeked status
    }
    if(currentPlayer->getIsTaxPrevented() == true)
    {
        currentPlayer->setIsTaxPrevented(false); // Reset the tax prevented status after the player finished his turn
    }
    if(currentPlayer->getIsBribePrevented() == true)
    {
        currentPlayer->setIsBribePrevented(false); // Reset the bribe prevented status after the player finished his turn
    }
    if(currentPlayer->getIsCoupPrevented() == true)
    {
        currentPlayer->setIsCoupPrevented(false); // Reset the coup prevented status after the player finished his turn
    }
    if(currentPlayer->getIsPeekedAndArrestPrevented() == true)
    {
        currentPlayer->setIsPeekedAndArrestPrevented(false); // Reset the peeked and arrest prevented status after the player finished his turn
    }
    if(currentPlayer->getIsSanctioned() == true)
    {
        currentPlayer->setIsSanctioned(false); // Reset the sanctioned status after the player finished his turn
    }
}

/**
 * @brief Move to the next player's turn.
 * This function sets the current player to not have a turn anymore, finds the next active player,
 * and sets them to have a turn. It uses modulo to wrap around the player list if necessary.
 *
 * @param none
 * @return void
 * @throws invalid_argument if no active players are found to take the next turn.
 */
void Game::nextTurn()
{
    Player *currentPlayer = current_player();
    resetPlayerStatus(currentPlayer);
    currentPlayer->setIsTurn(false);
    
    // Find next active player in the original players vector
    int nextIndex = currentPlayerIndex;
    bool foundNextPlayer = false;
    
    // Try to find next active player starting from current position + 1
    for (size_t i = 1; i < players.size(); i++)
    {
        nextIndex = (currentPlayerIndex + static_cast<int>(i)) % static_cast<int>(players.size());
        if (players[nextIndex]->getIsActive())
        {
            foundNextPlayer = true;
            break;
        }
    }
    
    if (foundNextPlayer)
    {
        currentPlayerIndex = nextIndex;
        players[currentPlayerIndex]->setIsTurn(true);
    }
    else
    {
        throw invalid_argument("No active players found to take the next turn.");
    }
}

/**
 * @brief Get the current player.
 * This function returns a pointer to the current player based on the currentPlayerIndex.
 *
 * @param none
 * @return Player* Pointer to the current Player object.
 * @throws none
 */
Player *Game::current_player()
{
    // Make sure currentPlayerIndex points to an active player
    if (currentPlayerIndex >= 0 && static_cast<size_t>(currentPlayerIndex) < players.size() && players[currentPlayerIndex]->getIsActive())
    {
        return players[currentPlayerIndex];
    }
    
    // If current index is invalid, find first active player
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->getIsActive())
        {
            currentPlayerIndex = i;
            return players[i];
        }
    }
    
    return nullptr; // No active players
}

/**
 * @brief Remove a player from the game.
 * This function sets the specified player's is_active flag to false and removes them from the players vector.
 * It also prints a message indicating that the player has been removed.
 *
 * @param player Pointer to the Player object to be removed.
 * @return void
 * @throws none
 */
void Game::removePlayer(Player *player) // Function to remove a player from the game
{
    bool wasCurrentPlayer = false;
    
    // Find and deactivate the player
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->getId() == player->getId())
        {
            // Check if this player currently has the turn
            wasCurrentPlayer = players[i]->getIsTurn();
            
            players[i]->setIsActive(false);     // Set the player to inactive
            players[i]->setIsTurn(false);       // Remove their turn status
            break;
        }
    }
    
    // If the removed player had the turn, use modified nextTurn logic
    if (wasCurrentPlayer)
    {
        vector<Player*> activePlayers = active_players();
        if (!activePlayers.empty())
        {
            // Find current position in active players and move to next
            int activeIndex = 0;
            for (size_t i = 0; i < activePlayers.size(); i++)
            {
                if (activePlayers[i]->getId() == activePlayers[currentPlayerIndex % activePlayers.size()]->getId())
                {
                    activeIndex = i;
                    break;
                }
            }
            
            // Move to next active player
            activeIndex = (activeIndex) % activePlayers.size(); // Stay at same position or wrap
            activePlayers[activeIndex]->setIsTurn(true);
            
            // Update currentPlayerIndex to match the new active player in original players list
            for (size_t i = 0; i < players.size(); i++)
            {
                if (players[i]->getId() == activePlayers[activeIndex]->getId())
                {
                    currentPlayerIndex = i;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Get the winner of the game.
 * This function checks if there is only one active player left in the game.
 * If so, it sets the winner_name and returns it; otherwise, it throws an exception.
 *
 * @param none
 * @return string The name of the winning player.
 * @throws invalid_argument if there is no winner yet (more than one active player).
 */
string Game::winner()
{
    vector<Player *> activePlayers = active_players();
    if (activePlayers.size() == 1)
    {
        winner_name = activePlayers[0]->getName();
        return winner_name;
    }
    else
    {
        throw invalid_argument("No winner yet!");
    }
}

/**
 * @brief Check if the game is finished.
 * This function checks if there is a winner by checking if the winner_name is not empty.
 * If there is no winner yet, it prints a message and returns false; otherwise, it sets isGameOver to true and returns true.
 *
 * @param none
 * @return bool True if the game is finished, false otherwise.
 * @throws none
 */
bool Game::isGameFinished()
{
    if (winner_name == "")
    {
        // no winner yet
        return false;
    }
    return isGameOver = true;
}


/**
 * @brief Get all players in the game (both active and inactive).
 * This function returns a vector containing pointers to all players in the game.
 *
 * @param none
 * @return vector<Player*> A vector of pointers to all Player objects.
 * @throws none
 */
vector<Player*> Game::getPlayers()
{
    return players;
}


/**
 * @brief Configure the game with a specified number of initial coins.
 * This function sets the initialCoins static variable to the specified value.
 *
 * @note The deafault value for initial coins is 50, but it can be changed using this method before creating the Game instance.
 * 
 * @param coins The number of coins to set as initial coins.
 * @return void
 * @throws invalid_argument if the coins parameter is negative.
 */
void Game::configure(int coins)
{
    if(coins < 0)
    {
        throw invalid_argument("Initial coins cannot be negative.");
    }
    initialCoins = coins;
}


/**
 * @brief Get the singleton instance of the Game class with player setup.
 * This function creates a new Game instance with the provided player setup if it doesn't already exist.
 *
 * @param numPlayers The number of players in the game.
 * @param playerNames A vector of strings containing the names of the players.
 * @return Game& Reference to the singleton Game instance.
 * @throws invalid_argument if parameters are invalid.
 */
Game& Game::getInstance(int numPlayers, const vector<string>& playerNames)
{
    if (!instance) {
        instance = new Game(numPlayers, playerNames);
    }
    return *instance;
}

/**
 * @brief Clean up the singleton instance.
 * This function deletes the singleton instance and resets the pointer to nullptr.
 * Should be called when returning to menu or exiting the application.
 *
 * @param none
 * @return void
 * @throws none
 */
void Game::cleanup()
{
    if (instance) {
        delete instance;
        instance = nullptr;
        cout << "Game instance cleaned up." << endl;
    }
}

//Messages in GUI functions:
/**
 * @brief Add a message to the game's message queue.
 * This function adds a message to the static messageQueue for later retrieval.
 *
 * @param message The message to be added to the queue.
 * @return void
 * @throws none
 */

void Game::addMessage(const std::string& message) 
{
    messageQueue.push(message);
}

/**
 * @brief Get the next message from the game's message queue.
 * This function retrieves and removes the front message from the static messageQueue.
 *
 * @return std::string The next message in the queue, or an empty string if the queue is empty.
 * @throws none
 */
std::string Game::getNextMessage() 
{
    if (!messageQueue.empty()) {
        std::string msg = messageQueue.front();
        messageQueue.pop();
        return msg;
    }
    return "";
}

/**
 * @brief Check if there are any messages in the game's message queue.
 * This function checks if the static messageQueue is not empty.
 *
 * @return bool True if there are messages, false otherwise.
 * @throws none
 */
bool Game::hasMessages() 
{
    return !messageQueue.empty();
}

/**
 * @brief Clear all messages from the game's message queue.
 * This function empties the static messageQueue, removing all messages.
 *
 * @return void
 * @throws none
 */
void Game::clearMessages() 
{
    while (!messageQueue.empty()) {
        messageQueue.pop();
    }
}

/**
 * @brief Clone a Character object based on the original's role.
 * This function creates a new Character object of the same type as the original,
 * but with a new owner and game instance.
 *
 * @note Used for the rule of 3 in Player class to deep copy the role.
 * 
 * @param original Pointer to the original Character object to clone.
 * @param newOwner Pointer to the new Player object that will own the cloned character.
 * @param newGame Pointer to the Game object that the cloned character will be part of.
 * @return Character* Pointer to the newly created cloned Character object, or nullptr if the role is unknown.
 * @throws none
 */
Character* cloneCharacter(const Character* original, Player* newOwner, Game* newGame)
{
    if (original == nullptr) return nullptr;
    
    string roleName = original->getRoleName();
    
    if (roleName == "Baron") {
        return new Baron(newOwner, newGame);
    }
    else if (roleName == "General") {
        return new General(newOwner, newGame);
    }
    else if (roleName == "Governor") {
        return new Governor(newOwner, newGame);
    }
    else if (roleName == "Judge") {
        return new Judge(newOwner, newGame);
    }
    else if (roleName == "Merchant") {
        return new Merchant(newOwner, newGame);
    }
    else if (roleName == "Spy") {
        return new Spy(newOwner, newGame);
    }
    
    return nullptr; // Unknown role type
}

/**
 * @brief Reset the singleton Game instance.
 * This function cleans up the current game instance, resets player states, and prepares for a new game.
 * It deletes all players and their roles, resets game state variables, and sets the instance pointer to nullptr.
 *
 * @return void
 * @throws none
 */
void Game::resetInstance()
{
    if (instance != nullptr) 
    {
        // Clean up all players
        for (Player* player : instance->players) 
        {
            delete player; // This will also delete the Character role
        }
        instance->players.clear();
        
        // Reset game state
        instance->currentPlayerIndex = 0;
        instance->coinsInBank = initialCoins;
        instance->isGameOver = false;
        instance->winner_name = "";
        
        // Delete the singleton instance
        delete instance;
        instance = nullptr;
    }
}