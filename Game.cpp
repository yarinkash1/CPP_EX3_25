// yarinkash1@gmail.com

#include "Game.hpp"

int Game::initialCoins = 50; // Default initial coins (can be changed by configure method)

Game* Game::instance = nullptr;  // Singleton instance pointer


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
    printf("Player %s was added to the game with a role of %s.\n", playerName.c_str(), role.c_str());
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
    cout << "New game started!" << endl;

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

    cout << "Game initialized with " << numPlayers << " players and " << coinsInBank << " coins in the bank." << endl;
    cout << "Adding players..." << endl;
    
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
 * @brief Constructor for the Game class with custom player names and roles(for debugging purposes).
 * Initializes the game with a specified number of players and assigns roles to each player based on the provided vectors.
 * 
 * @param playerNames A vector of strings containing the names of the players.
 * @param numPlayers The number of players in the game.
 * @param roles A vector of strings containing the roles assigned to each player.
 * @return void
 * @throws invalid_argument if the number of player names or roles does not match numPlayers.
 *
 */
Game::Game(const std::vector<std::string>& playerNames, int numPlayers, const std::vector<std::string>& roles) 
{
    if (playerNames.size() != static_cast<size_t>(numPlayers) || roles.size() != static_cast<size_t>(numPlayers)) 
    {
        throw std::invalid_argument("Number of players and roles must match numPlayers.");
    }

    // Step 1: Create Players with nullptr for role
    for (int i = 0; i < numPlayers; ++i) 
    {
        players.push_back(new Player(playerNames[i], nullptr));  // temporarily nullptr
    }

    // Step 2: Create Character roles now that Player* and Game* exist
    for (int i = 0; i < numPlayers; ++i) 
    {
        Character* character = createCharacterByRole(roles[i], players[i], this);
        players[i]->setRole(character);  // Assuming you have a setter to assign role after construction
    }

    // Set the first player as has turn:
    players[0]->setIsTurn(true);
}


/**
 * @brief Add a player to the game.
 * This function prompts the user for a player's name, creates a Player object with a random role(by using the createCharacterByRole function),
 * and adds the player to the game's player list.
 *
 * @param none
 * @return void
 * @throws none
 */
void Game::addPlayer()
{
    printf("Enter player %d name\n", numPlayers + 1);
    string name_of_player;
    getline(cin, name_of_player);

    Player *player = new Player(name_of_player, nullptr);

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
    printf("Player %s was added to the game with a role of %s.\n", name_of_player.c_str(), role.c_str());
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
    if(currentPlayer->getIsArrestPrevented() == true)
    {
        currentPlayer->setIsArrestPrevented(false); // Reset the arrest prevented status after the player finished his turn
    }
}

/**
 * @brief Move to the next player's turn.
 * This function sets the current player to not have a turn anymore, finds the next active player,
 * and sets them to have a turn. It uses modulo to wrap around the player list if necessary.
 *
 * @param none
 * @return void
 * @throws none
 */
void Game::nextTurn()
{
    Player *currentPlayer = current_player();

    resetPlayerStatus(currentPlayer); // Reset the current player's status at the end of their turn

    currentPlayer->setIsTurn(false); // Set the current player to not have turn anymore
    int num_of_active_players = active_players().size();
    // The modulo ensures that if the current player is the last one in the list, the next index wraps around to 0 (the first player):
    Player *nextPlayer = players[(currentPlayerIndex + 1) % num_of_active_players];
    currentPlayerIndex = (currentPlayerIndex + 1) % num_of_active_players; // Move to the next player
    nextPlayer->setIsTurn(true);                                           // Set the next player to have turn
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
    vector<Player *> activePlayers = active_players();
    return activePlayers[currentPlayerIndex];
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
    vector<Player *> activePlayers = active_players();
    for (size_t i = 0; i < activePlayers.size(); i++)
    {
        if (activePlayers[i]->getName() == player->getName())
        {
            activePlayers[i]->setIsActive(0);                                                       // Set the player to inactive
            players.erase(remove(players.begin(), players.end(), activePlayers[i]), players.end()); // Remove the player from the game
            cout << "Player " << activePlayers[i]->getName() << " has been removed from the game." << endl;
            break;
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
        cout << "No winner yet!" << endl;
        return false;
    }
    return isGameOver = true;
}

/**
 * @brief End the game and announce the winner.
 * This function checks if the game is over and prints the winner's name.
 *
 * @param none
 * @return void
 * @throws none
 */
void Game::endGame()
{
    if (isGameOver == true)
    {
        cout << "Game over!" << endl;
        cout << "The winner is: " << winner_name << endl;
    }
}

/**
 * @brief Reset the game to its initial state.
 * This function deletes all players, clears the players vector, resets the coins in the bank,
 * sets the current player index to 0, and marks the game as not over.
 *
 * @param none
 * @return void
 * @throws none
 */
void Game::resetGame()
{
    for (Player *p : players)
    {
        delete p;
    }
    players.clear();
    coinsInBank = 0;
    currentPlayerIndex = 0;
    isGameOver = false;
    winner_name = "";
    cout << "Game reset!" << endl;
}

/**
 * @brief Configure the game with a specified number of initial coins.
 * This function sets the initialCoins static variable to the specified value.
 *
 * @note The deafault value for initial coins is 50, but it can be changed using this method before creating the Game instance.
 * 
 * @param coins The number of coins to set as initial coins.
 * @return void
 * @throws none
 */
void Game::configure(int coins)
{
    initialCoins = coins;
}

/**
 * @brief Get the singleton instance of the Game class with specific player names and roles (for debugging purposes).
 * This function creates a new Game instance with the provided player names and roles if it doesn't already exist.
 *
 * @param playerNames A vector of strings containing the names of the players.
 * @param numPlayers The number of players in the game.
 * @param roles A vector of strings containing the roles assigned to each player.
 * @return Game* Pointer to the singleton Game instance.
 * @throws none
 */
Game* Game::getInstance(const vector<string>& playerNames,int numPlayers,const vector<string>& roles) 
{
    if (!instance) 
    {
        instance = new Game(playerNames, numPlayers, roles);
    }
    return instance;
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