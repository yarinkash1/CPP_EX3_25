#include "Game.hpp"

void Game::changeCoinsInBank(int amount)
{
    if (coinsInBank + amount < 0)
    {
        throw invalid_argument("Not enough coins in the bank.");
    }
    coinsInBank += amount;
}

Game::Game()
{
    cout << "New game started!" << endl;
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

    cout << "Game initialized with " << numPlayers << " players and " << coinsInBank << " coins in the bank." << endl;
    cout << "Adding players..." << endl;
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
int iteration = 0;
void Game::addPlayer()
{

    printf("Enter player %d name\n", iteration + 1);
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

vector<Player*> Game::active_players()
{
    vector<Player*> activePlayers;
    for (Player* player : players)
    {
        if (player->getIsActive() == 1) // 1 for true
        {
            activePlayers.push_back(player);
        }
    }
    return activePlayers;
}

int Game::getCoinsInBank()
{
    return coinsInBank;
}