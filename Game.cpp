#include "Game.hpp"

bool Game::changeCoinsInBank(int amount)
{
    if (coinsInBank + amount < 0)
    {
        return false;
    }
    coinsInBank += amount;
    return true;
}

Game::Game(int numPlayers, int coinsInBank)
{
    printf("Enter number of players\n");
    cin >> numPlayers;
    if (numPlayers < 2 || numPlayers > 6)
    {
        throw invalid_argument("Number of players must be between 2 and 6.");
    }
    printf("Enter number of coins in bank\n");
    cin >> coinsInBank;
    if (coinsInBank < 0)
    {
        throw invalid_argument("Number of coins in bank must be non-negative.");
    }

    this->numPlayers = numPlayers;
    this->coinsInBank = coinsInBank;
    this->currentPlayerIndex = 0;
    this->isGameOver = false;

    for(int i = 0; i < numPlayers; i++)
    {
        Game::addPlayer();
    }
}


Character* createCharacterByRole(const string& role,Player* player) 
{
    if (role == "Baron") return new Baron(player);
    else if (role == "General") return new General(player);
    else if (role == "Governor") return new Governor(player);
    else if (role == "Judge") return new Judge(player);
    else if (role == "Merchant") return new Merchant(player);
    else if (role == "Spy") return new Spy(player);
    else return nullptr; // or throw error
}

vector<string> roles_vector = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
void Game::addPlayer()
{
    int iteration = 0;
    printf("Enter player name\n");
    string name_of_player;
    cin >> name_of_player;
    Player* player = new Player(name_of_player, nullptr, 0);
    // do random from the six roles to assign to the player.
    random_device rd;
    mt19937 g(rd());
    shuffle(roles_vector.begin(), roles_vector.end(), g);
    // Assign the first role to the player
    string role = roles_vector[iteration];
    iteration++;
    Character* character = createCharacterByRole(role, player);
    player->setRole(character);
    players.push_back(player);
    printf("Player %s was added to the game with a role of %s.\n", name_of_player.c_str(), role.c_str());
}