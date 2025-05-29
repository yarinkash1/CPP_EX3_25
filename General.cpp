// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "General.hpp"
#include "Game.hpp"

/**
 * @brief Initializer list Constructor for the General class.
 * 
 * @param p Pointer to the Player who owns this General character.
 * @param g Pointer to the Game instance in which this General character exists.
 * @return General object
 * @throws None
 * 
 */
General::General(Player* p, Game* g) : Character(p, g){}

/**
 * @brief Prevents a coup on the target player.
 *
 * This action costs 5 coins
 * It sets the target player as coup prevented.
 *
 * @param target The target player to prevent the coup on.
 * @return void
 * @throws None
 */
void General::preventCoup(Player &target)
{
    owner->removeNumCoins(5); // Remove 5 coins from the owner
    game->changeCoinsInBank(+5); // Add 5 coins to the bank
    target.setIsCoupPrevented(true); // Set the target player as coup prevented
    Game::addMessage(owner->getName() + " has prevented a coup on " + target.getName());
    cout << owner->getName() << " has prevented a coup on " << target.getName() << std::endl;
    // let gui handle the next turn here
    return;
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the preventCoup() method of the General.
 *
 * @param None
 * @return void
 * @throws None
 */
void General::Action()
{
    // Empty - General doesn't use parameterless action
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the preventCoup() method of the General.
 *
 * @note This function allows the player to prevent a coup on another player(target).
 * @param target Pointer to the Player object that is being targeted for the coup prevention.
 * @return void
 * @throws None
 */
void General::Action(Player* target)
{
    if (target != nullptr)
    {
        preventCoup(*target); // Delegate to existing preventCoup method
    }
    else
    {
        Game::addMessage("No target selected for General action.");
    }
}

/**
 * @brief Prompts the player to choose an action and executes the selected action.
 *
 * @note This function is obsolete (the GUI handles the action selection now), but it is kept for terminal playing option.
 * This function displays a list of available actions and allows the player to select one.
 * It handles invalid input and retries if necessary.
 *
 * @param None
 * @return void
 * @throws None
 */
void General::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Prevent Coup" << endl;
    cout << "8. Skip turn" << endl;
    cin >> actionType;

    if (actionType < 1 || actionType > 8)
    {
        cout << "Invalid action type. Please try again." << endl;
        chooseAction(); // Retry if invalid
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
            Action(); // Call the Action method for Prevent Coup
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