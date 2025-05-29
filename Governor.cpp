// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"
#include "Game.hpp"

/**
 * @brief Initializer list constructor for the Governor class.
 *
 * This constructor initializes the Governor object with a player and a game instances.
 *
 * @param p Pointer to the Player object that owns this character.
 * @param g Pointer to the Game object that this character is part of.
 * @return Governor object.
 * @throws None
 */
Governor::Governor(Player *p, Game *g) : Character(p, g) {}

/**
 * @brief Performs the "tax" action, granting the player 3 coins from the bank.
 *
 * This action is free (costs nothing) and may be blocked by a sanction or by a Governor.
 *
 * @param None
 * @return void
 * @throws None
 */
void Governor::tax()
{
    game->changeCoinsInBank(-3); // Deduct 3 coins from the bank
    owner->addNumCoins(3);       // Add 3 coins to the player's total
    Game::addMessage(" -- Tax action performed --");
    game->nextTurn(); // Move to the next player's turn
    return;
}

/**
 * @brief Cancels the tax action on a target player.
 *
 * This action is free (costs nothing).
 * Sets the target player as tax prevented, preventing them from performing tax.
 * @note The action will be blocked in the GUI if the player is already tax prevented.
 *
 * @param target The target player to cancel the tax on.
 * @return void
 * @throws None
 */
void Governor::cancelTax(Player &target)
{
    target.setIsTaxPrevented(true); // Set the target player as tax prevented
    Game::addMessage(owner->getName() + " has canceled the tax on " + target.getName());
    // let main handle the next turn here
    return;
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the cancelTax() method of the Governor.
 *
 * @param None
 * @return void
 * @throws None
 */
void Governor::Action()
{
    tax(); // calls the governor's tax method
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the cancelTax() method of the Governor.
 *
 * @note This function allows the player to cancel tax on another player(target).
 * @param target Pointer to the Player object that is being targeted for the tax cancellation.
 * @return void
 * @throws None
 */
void Governor::Action(Player* target) 
{
    if (target != nullptr) 
    {
        cancelTax(*target); // Calls public method
    } 
    else 
    {
        Game::addMessage("No target selected for Governor action.");
    }
}

/**
 * @brief Prompts the player to choose an action and executes the selected action.
 *
 * This function displays a list of available actions and allows the player to select one.
 * It handles invalid input and retries if necessary.
 *@note This function is obsolete (the GUI handles the action selection now), but it is kept for terminal playing option.
 * 
 * @param None
 * @return void
 * @throws None
 */
void Governor::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax(Governor)" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Cancel Tax" << endl;
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
        Action(); // Call the Action method for Governor Tax
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