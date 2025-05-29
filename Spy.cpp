// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Spy.hpp"
#include "Game.hpp"

/**
 * @brief Initializer list constructor for the Spy class.
 *
 * This constructor initializes the Spy object with a player and a game instances.
 *
 * @param p Pointer to the Player object that owns this character.
 * @param g Pointer to the Game object that this character is part of.
 * @return Spy object.
 * @throws None
 */
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
        Game::addMessage("You have already peeked at a player's coins this turn. Choose a different action.");
        // Remove: chooseAction();
        // Instead, just return - GUI will handle showing the message
        return;
    }
    else
    {
        int target_num_coins = target.getCoins();
        Game::addMessage("You prevented arrest on " + target.getName());
        Game::addMessage("You peeked at " + target.getName() + "'s coins: " + std::to_string(target_num_coins));
        target.setIsPeekedAndArrestPrevented(true);
        alreadyPeeked = true;

        // DON'T call chooseAction() here - let GUI handle the extra turn
        // DON'T call game->nextTurn() - Spy should keep the turn
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
    // Empty - Spy doesn't use parameterless action
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the peekAndPreventArrest() method of the Spy.
 *
 * @note This function allows the player to peek at another player's coins and prevent their arrest.
 * @param target Pointer to the Player object that is being targeted for peeking and preventing arrest.
 * @return void
 * @throws None
 */
void Spy::Action(Player *target)
{
    if (target != nullptr)
    {
        peekAndPreventArrest(*target); // Calls public method
    }
    else
    {
        Game::addMessage("No target selected for Spy action.");
    }
}

/**
 * @brief Prompts the player to choose an action and executes the selected action.
 *
 * This function displays a list of available actions and allows the player to select one.
 * It handles invalid input and retries if necessary.
 * @note This function is obsolete (the GUI handles the action selection now), but it is kept for terminal playing option.
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
        chooseAction(); // Retry if invalid
        break;
    }
}