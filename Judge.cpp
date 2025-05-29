// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Judge.hpp"
#include "Game.hpp"

/**
 * @brief Initializer list constructor for the Judge class.
 *
 * This constructor initializes the Judge object with a player and a game instances.
 *
 * @param p Pointer to the Player object that owns this character.
 * @param g Pointer to the Game object that this character is part of.
 * @return Judge object.
 * @throws None
 */
Judge::Judge(Player* p, Game* g) : Character(p, g){}

/**
 * @brief Cancels the bribe action on a target player.
 *
 * This action is free (costs nothing). 
 * Sets the target player as bribe prevented, thus when clicking bribe not getting another turn and losing the coins paid.
 *
 * @param target The target player to cancel the bribe on.
 * @return void
 * @throws None
 */
void Judge::cancelBribe(Player &target)
{
    target.setIsBribePrevented(true); // Set the target player as bribe prevented
    Game::addMessage(owner->getName() + " has canceled the bribe on " + target.getName());
    cout << owner->getName() << " has canceled the bribe on " << target.getName() << endl;
    // let main handle the next turn here
    return;
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the cancelBribe() method of the Judge.
 *
 * @param None
 * @return void
 * @throws None
 */
void Judge::Action()
{
    cancelBribe(*owner);
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the cancelBribe() method of the Judge.
 *
 * @note This function allows the player to cancel bribe on another player(target).
 * @param target Pointer to the Player object that is being targeted for the bribe cancellation.
 * @return void
 * @throws None
 */
void Judge::Action(Player* target) 
{
    if (target != nullptr) 
    {
        cancelBribe(*target); // Calls private method
    } else 
    {
        Game::addMessage("No target selected for Judge action.");
    }
}

/**
 * @brief Prompts the player to choose an action and executes the selected action.
 *
 * This function displays a list of available actions and allows the player to select one.
 * It handles invalid input and retries if necessary.
 * @note this function is Obsolete (the GUI handles the action selection now), but it is kept for terminal playing option.
 *
 * @param None
 * @return void
 * @throws None
 */
void Judge::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Cancel Bribe" << endl;
    cout << "8. Skip turn" << endl;
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
        Action(); // Call the Action method for cancel bribe
        break;
    case 8:
        // Skip turn logic
        game->nextTurn(); // Move to the next player's turn
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
        chooseAction(); // Retry if invalid
        break;
    }
}
