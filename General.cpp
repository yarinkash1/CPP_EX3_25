// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "General.hpp"
#include "Game.hpp"

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
    cout << owner->getName() << " has prevented a coup on " << target.getName() << std::endl;
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
    preventCoup(*owner);
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
    }
}