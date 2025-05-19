#include "Player.hpp"
#include "Character.hpp"
#include "Game.hpp"

Character::Character(Player *p, Game *g) : owner(p), game(g) {}

void Character::gather()
{
    game->changeCoinsInBank(-1); // Deduct 1 coin from the bank
    owner->addNumCoins(1);       // Add 1 coin to the player's coins
    std::cout << "Gather action performed." << std::endl;
}

void Character::tax()
{
    game->changeCoinsInBank(-2); // Deduct 2 coins from the bank
    owner->addNumCoins(2);       // Add 2 coins to the player's coins
    std::cout << "Tax action performed." << std::endl;
}

void Character::bribe()
{
    owner->removeNumCoins(4);    // Remove 4 coins from the player's coins
    game->changeCoinsInBank(+4); // Add 4 coins to the bank
    std::cout << "Bribe action performed.\n"
              << std::endl;
    std::cout << "Player " << owner->getName() << " has another turn" << std::endl;
}

void Character::arrest(Player &player)
{
    
    Player *targetPlayer = target_player(); // Get the target player
    if (targetPlayer == nullptr)
    {
        std::cout << "Invalid target player." << std::endl;
        return;
    }
    targetPlayer->setIsArrested(true); // Set the target player as arrested
    targetPlayer->removeNumCoins(1);   // Remove 1 coin from the target player
    game->changeCoinsInBank(1);        // Add 1 coin to the bank

    std::cout << "Arrest action performed on player: " << targetPlayer->getName() << std::endl;
}

void Character::sanction(Player &player)
{
    Player *targetPlayer = target_player(); // Get the target player
    if (targetPlayer == nullptr)
    {
        std::cout << "Invalid target player." << std::endl;
        return;
    }
    targetPlayer->setIsSanctioned(true); // Set the target player as sanctioned
    owner->removeNumCoins(3);     // Remove 3 coins from the target player
    game->changeCoinsInBank(3);   // Add 3 coins to the bank

    std::cout << "Sanction action performed on player: " << targetPlayer->getName() << std::endl;
}

void Character::coup(Player &player)
{
    Player *targetPlayer = target_player(); // Get the target player
    if (targetPlayer == nullptr)
    {
        std::cout << "Invalid target player." << std::endl;
        return;
    }
    
    owner->removeNumCoins(7);        // Remove 7 coins from the player's coins
    game->changeCoinsInBank(7);      // Add 7 coins to the bank
    targetPlayer->setIsActive(false); // Set the target player as inactive
    game->removePlayer(targetPlayer); // Remove the target player from the game


    std::cout << "Coup action performed on player: " << targetPlayer->getName() << std::endl;
}

Player *Character::target_player()
{
    vector<Player *> active_players = game->active_players();
    cout << "Choose a player to arrest from the list (1 - " << active_players.size() << endl;
    for (int i = 0; i < active_players.size(); i++)
    {
        cout << i + 1 << ": " << active_players[i]->getName() << endl;
    }
    int choice;
    cin >> choice;

    Player *currentPlayer = game->current_player();
    int selected_char_index = -1;
    for (int i = 0; i < active_players.size(); i++)
    {
        if (active_players[i]->getId() == currentPlayer->getId())
        {
            selected_char_index = i;
            break;
        }
    }

    if (choice < 1 || choice > active_players.size())
    {
        cout << "Invalid choice. Please try again." << endl;
        return nullptr;
    }
    else if (choice == selected_char_index + 1)
    {
        cout << "You cannot target yourself." << endl;
        return nullptr;
    }
    else
    {
        Player *targetPlayer = active_players[choice - 1];
        cout << "You have selected player: " << targetPlayer->getName() << endl;
        return targetPlayer; // Return the target player
    }
}
