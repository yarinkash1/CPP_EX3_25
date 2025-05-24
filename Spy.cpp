// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Spy.hpp"
#include "Game.hpp"

Spy::Spy(Player *p, Game *g) : Character(p, g) {}

/**
 * @brief Peeks at the target player's coins and prevents their arrest for the next turn.
 *
 * This action allows the Spy to peek at the target player's coins and prevent their arrest for the next turn.
 * It sets the target player as arrest prevented.
 * This action is free (costs nothing) and isn't considered a turn action(cannot be performed twice in a row).
 *
 * @param target The target player to peek at and prevent arrest.
 * @return void
 * @throws None
 */
void Spy::peekAndPreventArrest(Player &target)
{
    if (alreadyPeeked == true)
    {
        cout << "You have already peeked at a player's coins this turn. Choose a different action" << endl;
        chooseAction(); // Prompt the player to choose another action
    }
    else
    {
        int target_num_coins = target.getCoins(); // Get the number of coins of the target player
        cout << target.getName() << " has " << target_num_coins << " coins." << endl;
        target.setIsArrestPrevented(true); // Set the target player as arrest prevented
        cout << owner->getName() << " has peeked at " << target.getName() << "'s coins and prevented their arrest for their next turn." << endl;
        chooseAction();       // Prompt the player to choose another action
        alreadyPeeked = true; // Set the alreadyPeeked flag to true
    }
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the peekAndPreventArrest() method of the Spy.
 *
 * @param None
 * @return void
 * @throws None
 */
void Spy::Action()
{
    peekAndPreventArrest(*owner);
}

/**
 * @brief Prompts the player to choose an action and executes the selected action.
 *
 * This function displays a list of available actions and allows the player to select one.
 * It handles invalid input and retries if necessary.
 *
 * @param None
 * @return void
 * @throws None
 */
void Spy::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Peek Cards and Prevent Arrest" << endl;
    cout << "8. Skip Turn" << endl;
    cin >> actionType;

    if (actionType < 1 || actionType > 8)
    {
        cout << "Invalid action type. Please try again." << endl;
        return;
    }

    switch (actionType)
    {
    case 1:
        this->gather();
        break;
    case 2:
        this->tax();
        break;
    case 3:
        this->bribe();
        break;
    case 4:
        this->arrest();
        break;
    case 5:
        this->sanction();
        break;
    case 6:
        this->coup();
        break;
    case 7:
        Action(); // Call the Action method for peek cards and prevent arrest
        break;
    case 8:
        game->nextTurn(); // Skip the turn
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
    }
}