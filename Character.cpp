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
        Game::addMessage("You are sanctioned and cannot perform the gather action.");
        // cout << "You are sanctioned and cannot perform the gather action." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        game->changeCoinsInBank(-1); // Deduct 1 coin from the bank
        owner->addNumCoins(1);       // Add 1 coin to the player's coins
        Game::addMessage("-- Gather action performed. --");
        // cout << " -- Gather action performed. --" << endl;
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
        Game::addMessage("You are sanctioned and cannot perform the tax action because you are blocked.");
        // cout << "You are sanctioned and cannot perform the tax action because you are blocked." << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        game->changeCoinsInBank(-2); // Deduct 2 coins from the bank
        owner->addNumCoins(2);       // Add 2 coins to the player's coins
        Game::addMessage("-- Tax action performed. --");
        // cout << "-- Tax action performed. --" << endl;
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
        Game::addMessage("You don't have enough coins to perform the bribe action. Choose another action.");
        // cout << "You don't have enough coins to perform the bribe action. choose another action" << endl;
        return;
    }
    else
    {
        if (owner->getIsBribePrevented() == true)
        {
            Game::addMessage("You were bribe prevented and cannot perform the bribe action.");
            //  cout << "You were bribe prevented and cannot perform the bribe action." << endl;
            game->changeCoinsInBank(+4); // Add 4 coins to the bank the bank
            owner->removeNumCoins(4);    // Remove 4 coins from the player's coins
            Game::addMessage("You lost the 4 coins you payed.");
            // cout << "You lost the 4 coins you payed." << endl;
            return;
        }
        else
        {
            owner->removeNumCoins(4);    // Remove 4 coins from the player's coins
            game->changeCoinsInBank(+4); // Add 4 coins to the bank
            Game::addMessage("-- Bribe action performed. Player " + owner->getName() + " has another turn --");
        }
        return;
    }
}

/**
 * @brief Performs the "arrest" action on a target player.
 *
 * This action costs nothing and can be blocked or altered by specific roles.
 * It sets the target player as arrested causing them to lose 1 coin and giving it to the player who performed the arrest action on them.
 *
 * @note If the target player is a Merchant, he pays 2 coins to the bank instead of losing them to the player performing arrest on them.
 * If the target player is a General, he gets his coin back.
 * A spy can prevent a player from performing arrest on his next turn.
 *
 *
 * @param None
 * @return void
 * @throws None
 */
void Character::arrest(Player *target)
{
    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            Game::addMessage("-- Invalid target player. --");
            // cout << "-- Invalid target player. --" << endl;
            return;
        }
    }
    if(target->getRole()->getRoleName() == "Merchant")
    {
        owner->removeNumCoins(2); // Remove 2 coins from the owner
        game->changeCoinsInBank(+2); // Add 2 coins to the bank
        Game::addMessage(target->getName() + " is a Merchant and pays 2 coins to the bank instead of losing them to you.");
        game->nextTurn();
        return;
    }
    // Transfer 1 coin from target to arresting player
    target->removeNumCoins(1); // Remove 1 coin from target
    owner->addNumCoins(1);     // Give 1 coin to arresting player
    if(target->getRole()->getRoleName() == "General")
    {
            target->addNumCoins(1); // Add 1 coin to the target
            Game::addMessage(target->getName() + " is a General and gets his coin back.");
    }
    target->setIsArrested(true);
    // Rest of arrest logic using 'target' parameter
    Game::addMessage("-- Arresting " + target->getName() + " --");
    // cout << "-- Arresting " << target->getName() << " --" << endl;

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
void Character::sanction(Player *target)
{
    if (owner->getCoins() < 3)
    {
        Game::addMessage("You don't have enough coins to perform the sanction action.");
        // cout << "You don't have enough coins to perform the sanction action." << endl;
        return;
    }

    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            Game::addMessage("-- Invalid target player. --");
            //  cout << "-- Invalid target player. --" << endl;
            return;
        }
    }

    // Rest of sanction logic
    owner->removeNumCoins(3);
    game->changeCoinsInBank(+3);
    Game::addMessage("-- Sanctioning " + target->getName() + " --");
    //  cout << "-- Sanctioning " << target->getName() << " --" << endl;
    target->setIsSanctioned(true); // Example logic

    if(target->getRole()->getRoleName() == "Judge")
    {
        owner->removeNumCoins(1); // Remove 1 coin from the owner
        game->changeCoinsInBank(+1); // Add 1 coin to the bank
        Game::addMessage(target->getName() + " is a Judge and " +owner->getName() + " needs to pay another coin to the bank.");
    }
    else if(target->getRole()->getRoleName() == "Baron")
    {
        target->addNumCoins(1); // Add 1 coin to the target
        Game::addMessage(target->getName() + " is a Baron and gets 1 coin as a compensation.");
    }

    game->nextTurn();
}

/**
 * @brief Performs the "coup" action on a target player.
 *
 * This action costs 7 coins and eliminates the target player from the game.
 * It can be blocked by a General.
 * If a player has more than 10 coins he is forced to perform a coup action.
 *
 * @note 
 *
 * @param target Pointer to the Player object that is being targeted for the coup.
 * @return void
 * @throws None
 */
void Character::coup(Player *target)
{
    if (owner->getCoins() < 7)
    {
        Game::addMessage("You don't have enough coins to perform the coup action.");
        cout << "You don't have enough coins to perform the coup action." << endl;
        return;
    }

    if (target == nullptr)
    {
        // Console version - use existing target_player() logic
        target = target_player();
        if (target == nullptr)
        {
            Game::addMessage("-- Invalid target player. --");
            cout << "-- Invalid target player. --" << endl;
            return;
        }
    }

    // Handle General preventing coup on himself
    if (target->getRole()->getRoleName() == "General")
    {
        if(target->getCoins()>=5)
        {
            target->removeNumCoins(5); // Remove 5 coins from the target
            game->changeCoinsInBank(+5); // Add 5 coins to the bank
            Game::addMessage("General has prevented coup by paying 5 coins.");
            return; // Coup action blocked
        }
        game->nextTurn();
        return; // Coup action blocked
    }

    // Perform coup
    owner->removeNumCoins(7);
    game->changeCoinsInBank(+7);
    Game::addMessage("-- Couping " + target->getName() + " --");
    target->setIsActive(false); // Eliminate player


    try
    {
        string winnerName = game->winner(); // This will succeed if only 1 player remains
        Game::addMessage("Game Over! Winner: " + winnerName);
        cout << "Game Over! Winner: " << winnerName << endl;
        
        return; // Don't call nextTurn() - let the popup handle next steps
    }
    catch (const invalid_argument &e)
    {
        // No winner yet (more than 1 active player), continue game normally
        Game::addMessage(owner->getName() + " eliminated " + target->getName() + "!");
        cout << owner->getName() << " eliminated " << target->getName() << "!" << endl;
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
        Game::addMessage("No valid targets available.");
        cout << "No valid targets available." << endl;
        return nullptr;
    }

    // Game::addMessage("Choose a target player from the list:");
    cout << "Choose a player to target from the list (1 - " << valid_targets.size() << ")" << endl;
    for (size_t i = 0; i < valid_targets.size(); i++)
    {
        cout << i + 1 << ": " << valid_targets[i]->getName() << endl;
    }

    int choice;
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(valid_targets.size()))
    {
        Game::addMessage("Invalid choice. Please try again.");
        cout << "Invalid choice. Please try again." << endl;
        return nullptr;
    }
    else
    {
        Player *targetPlayer = valid_targets[choice - 1];
        Game::addMessage("-- You have selected player: " + targetPlayer->getName() + " --");
        cout << "-- You have selected player: " << targetPlayer->getName() << " --" << endl;
        return targetPlayer; // Return the target player
    }
}
