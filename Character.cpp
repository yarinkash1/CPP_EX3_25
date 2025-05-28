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
    if (owner->getCoins() < 4)
    {
        cout << "You don't have enough coins to perform the bribe action. choose another action" << endl;
        chooseAction(); // Prompt the player to choose another action
        return;
    }
    else
    {
        if (owner->getIsBribePrevented() == true)
        {
            cout << "You were bribe prevented and cannot perform the bribe action." << endl;
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
void Character::arrest(Player* target)
{
    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            cout << "-- Invalid target player. --" << endl;
            return;
        }
    }
    
    // Rest of arrest logic using 'target' parameter
    cout << "-- Arresting " << target->getName() << " --" << endl;
    target->setIsArrestPrevented(true); // Example logic
    game->nextTurn();
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
void Character::sanction(Player* target)
{
    if (owner->getCoins() < 3)
    {
        cout << "You don't have enough coins to perform the sanction action." << endl;
        return;
    }
    
    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            cout << "-- Invalid target player. --" << endl;
            return;
        }
    }
    
    // Rest of sanction logic
    owner->removeNumCoins(3);
    game->changeCoinsInBank(+3);
    cout << "-- Sanctioning " << target->getName() << " --" << endl;
    target->setIsSanctioned(true); // Example logic
    game->nextTurn();
}

void Character::coup(Player* target)
{
    if (owner->getCoins() < 7)
    {
        cout << "You don't have enough coins to perform the coup action." << endl;
        return;
    }
    
    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            cout << "-- Invalid target player. --" << endl;
            return;
        }
    }
    
    // Rest of coup logic
    owner->removeNumCoins(7);
    game->changeCoinsInBank(+7);
    cout << "-- Couping " << target->getName() << " --" << endl;
    target->setIsActive(false); // Example logic - eliminate player
    game->nextTurn();
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
    Player *currentPlayer = game->current_player();
    
    // Create a filtered list excluding the current player
    vector<Player *> valid_targets;
    for (size_t i = 0; i < active_players.size(); i++)
    {
        if (active_players[i]->getId() != currentPlayer->getId())
        {
            valid_targets.push_back(active_players[i]);
        }
    }
    
    // Check if there are any valid targets
    if (valid_targets.empty())
    {
        cout << "No valid targets available." << endl;
        return nullptr;
    }
    
    cout << "Choose a player to target from the list (1 - " << valid_targets.size() << ")" << endl;
    for (size_t i = 0; i < valid_targets.size(); i++)
    {
        cout << i + 1 << ": " << valid_targets[i]->getName() << endl;
    }
    
    int choice;
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(valid_targets.size()))
    {
        cout << "Invalid choice. Please try again." << endl;
        return nullptr;
    }
    else
    {
        Player *targetPlayer = valid_targets[choice - 1];
        cout << "-- You have selected player: " << targetPlayer->getName() << " --" << endl;
        return targetPlayer; // Return the target player
    }
}
