// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Game.hpp"

/**
 * @brief Initializer constructor for the Character class.
 *
 * This constructor initializes the Character object with a player and a game instances.
 *
 * @param p Pointer to the Player object that owns this character.
 * @param g Pointer to the Game object that this character is part of.
 * @return Character object.
 * @throws None
 */
Character::Character(Player *p, Game *g) : owner(p), game(g) {}

/**
 * @brief Performs the "gather" action, granting the player 1 coin from the bank.
 *
 * This action is free (costs nothing) and may be blocked by a sanction.
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::gather()
{
    if (owner->getIsSanctioned() == true)
    {
        cout << "You are sanctioned and cannot perform the gather action." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        game->changeCoinsInBank(-1); // Deduct 1 coin from the bank
        owner->addNumCoins(1);       // Add 1 coin to the player's coins
        cout << " -- Gather action performed. --" << endl;
        game->nextTurn(); // Move to the next player's turn
        return;
    }
}

/**
 * @brief Performs the "tax" action, granting the player 2 coins from the bank.
 *
 * This action is free (costs nothing) and may be blocked by a sanction or by a Governor.
 *
 * @note If a Governor performs the tax action, he is granted 3 coins instead of 2.
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::tax()
{
    if (owner->getIsSanctioned() == true || owner->getIsTaxPrevented() == true)
    {
        cout << "You are sanctioned and cannot perform the tax action because you are blocked." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        game->changeCoinsInBank(-2); // Deduct 2 coins from the bank
        owner->addNumCoins(2);       // Add 2 coins to the player's coins
        cout << "-- Tax action performed. --" << endl;
        game->nextTurn(); // Move to the next player's turn
        return;
    }
}

/**
 * @brief Performs the "bribe" action, granting the player another turn.
 *
 * This action costs 4 coins and can be blocked by a Judge.
 *
 * @note If a judge blocks the bribe action of another player,
 * it causes the player who performed the bribe action to lose the 4 coins he payed.
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::bribe()
{
    if (owner->getIsBribePrevented() == true)
    {
        cout << "You are blocked and cannot perform the bribe action." << endl;
        game->changeCoinsInBank(+4); // Add 4 coins to the bank the bank
        owner->removeNumCoins(4);    // Remove 4 coins from the player's coins
        cout << "You lost the 4 coins you payed." << endl;
        return;
    }
    else
    {
        owner->removeNumCoins(4);    // Remove 4 coins from the player's coins
        game->changeCoinsInBank(+4); // Add 4 coins to the bank
        cout << "-- Bribe action performed. --\n"
             << endl;
        cout << "-- Player " << owner->getName() << " has another turn --" << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    game->nextTurn(); // Move to the next player's turn once he finished bribing
    return;
}

/**
 * @brief Performs the "arrest" action on a target player.
 *
 * This action costs nothing and can be blocked or altered by specific roles.
 * It sets the target player as arrested causing them to lose 1 coin and giving it to the player who performed the arrest action on them.
 *
 * @note If the target player is a Merchant, he pays 2 coins to the bank instead of losing them to the player performing bribe on them.
 * If the target player is a General, he gets his coin back.
 * A spy can prevent a player from performing arrest on his next turn.
 *
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::arrest()
{
    if (owner->getIsArrestPrevented() == true || owner->getIsPeekedAndArrestPrevented() == true)
    {
        cout << "You are blocked and cannot perform the arrest action until your next turn." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        Player *targetPlayer = target_player(); // Get the target player
        if (targetPlayer == nullptr)
        {
            cout << "Invalid target player." << endl;
            return;
        }
        if (targetPlayer->getRole()->getRoleName() == "Merchant")
        {
            targetPlayer->removeNumCoins(2); // Remove 2 coins from the target player
            game->changeCoinsInBank(+2);     // Add 2 coins to the bank
            cout << "-- Merchant arrested, he pays 2 coins to the bank instead of the player who targeted him --" << endl;
            game->nextTurn(); // Move to the next player's turn
            return;
        }
        else
        {
            targetPlayer->removeNumCoins(1); // Remove 1 coin from the target player
            game->changeCoinsInBank(1);      // Add 1 coin to the bank
            owner->addNumCoins(1);           // Add 1 coin to the player who performed the arrest action
            cout << "-- Arrest action performed on player: --" << targetPlayer->getName() << endl;
        }

        if (targetPlayer->getRole()->getRoleName() == "General")
        {
            targetPlayer->addNumCoins(1); // Add 1 coin to the target player(gets his coin back)
            cout << "-- General arrested, he gets his coin back --" << endl;
        }

        game->nextTurn(); // Move to the next player's turn
        return;
    }
}

/**
 * @brief Performs the "sanction" action on a target player.
 *
 * This action costs 3 coins and can be altered by specific roles.
 * It sets the target player as sanctioned preventing them from performing economic actions(tax and gather).
 *
 * @note If the target player is a Baron, he gets 1 coin as a compensation.
 * If the target player is a Judge, the person who performed the saction action on him needs to pay another coin to the bank.
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::sanction()
{
    Player *targetPlayer = target_player(); // Get the target player
    if (targetPlayer == nullptr)
    {
        std::cout << "Invalid target player." << std::endl;
        return;
    }

    owner->removeNumCoins(3);   // Remove 3 coins from the target player
    game->changeCoinsInBank(3); // Add 3 coins to the bank
    cout << "-- Sanction action performed on player: --" << targetPlayer->getName() << endl;

    if (targetPlayer->getRole()->getRoleName() == "Baron")
    {
        game->changeCoinsInBank(-1);  // Deduct 1 coin from the bank
        targetPlayer->addNumCoins(1); // Add 1 coin to the target player as a compensation
        cout << "-- Baron sanctioned, he gets 1 coin as a compensation --" << endl;
    }
    else if (targetPlayer->getRole()->getRoleName() == "Judge")
    {
        owner->removeNumCoins(1);   // Remove 1 coin from the player who performed the sanction action
        game->changeCoinsInBank(1); // Add 1 coin to the bank
        cout << "-- Judge sanctioned, you need to pay another coin to the bank --" << endl;
    }

    targetPlayer->setIsSanctioned(true); // Set the target player as sanctioned
    game->nextTurn();                    // Move to the next player's turn
    return;
}

/**
 * @brief Performs the "coup" action on a target player.
 *
 * This action costs 7 coins and can be blocked by a General.
 * It sets the target player as inactive and removes them from the game if he can't block it or a merchant didn't prevent the current coup.
 *
 * @note A merchant can prevent a coup on another player or on himself by paying 5 coins to the bank.
 * If a player has 10 coins or more, he is forced to perform a coup on another player.
 *
 * @param None
 * @return void
 * @throws None
 *
 */
void Character::coup()
{
    if (owner->getIsCoupPrevented() == true)
    {
        cout << "You are blocked and cannot perform the coup action." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        Player *targetPlayer = target_player(); // Get the target player
        if (targetPlayer == nullptr)
        {
            cout << "-- Invalid target player. --" << endl;
            return;
        }
        else if (targetPlayer->getRole()->getRoleName() == "Merchant")
        {
            owner->removeNumCoins(5);    // Remove 5 coins from the player's coins
            game->changeCoinsInBank(+5); // Deduct 5 coins from the bank

            cout << "-- Merchant prevented the coup action on himself --" << endl;
        }
        else
        {
            // Check if the merchant wants to prevent the coup action on the player it was performed on
             vector <Player*> active_players_vec = game->active_players();
            for (size_t i = 0; i < active_players_vec.size(); i++)
            {
                if(active_players_vec[i]->getRole()->getRoleName() == "Merchant")
                {
                    cout << " Do you want to prevent the coup action on " << targetPlayer->getName() << " ? (y/n)" << endl;
                    char choice;
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y')
                    {
                        active_players_vec[i]->removeNumCoins(5);    // Remove 5 coins from the player's coins
                        game->changeCoinsInBank(+5); // Deduct 5 coins from the bank
                        cout << "-- Merchant prevented the coup action on " << targetPlayer->getName() << " --" << endl;
                        game->nextTurn(); // Move to the next player's turn
                        return;
                    }
                }
            }

            targetPlayer->setIsActive(false); // Set the target player as inactive
            game->removePlayer(targetPlayer); // Remove the target player from the game

            cout << "-- Coup action performed on player: --" << targetPlayer->getName() << endl;
        }

        owner->removeNumCoins(7);   // Remove 7 coins from the player's coins
        game->changeCoinsInBank(7); // Add 7 coins to the bank

        game->nextTurn(); // Move to the next player's turn
        return;
    }
}

/**
 * @brief Prompts the player to select a target player to perform an action on.
 *
 * This function displays a list of active players and allows the user to select one.
 * It ensures that the selected player is not the current player and is valid.
 *
 * @param None
 * @return Player* Pointer to the selected target player, or nullptr if the selection is invalid.
 * @throws None
 */
Player *Character::target_player()
{
    vector<Player *> active_players = game->active_players();
    cout << "Choose a player to arrest from the list (1 - " << active_players.size() << endl;
    for (size_t i = 0; i < active_players.size(); i++)
    {
        cout << i + 1 << ": " << active_players[i]->getName() << endl;
    }
    int choice;
    cin >> choice;

    Player *currentPlayer = game->current_player();
    int selected_char_index = -1;
    for (size_t i = 0; i < active_players.size(); i++)
    {
        if (active_players[i]->getId() == currentPlayer->getId())
        {
            selected_char_index = i;
            break;
        }
    }

    if (choice < 1 || choice > static_cast<int>(active_players.size()))
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
        cout << "-- You have selected player: " << targetPlayer->getName() << " --" << endl;
        return targetPlayer; // Return the target player
    }
}
