// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Merchant.hpp"
#include "Game.hpp"

Merchant::Merchant(Player* p, Game* g) : Character(p, g){}

/**
 * @brief Adds a coin to the Merchant's total if he has 3 or more coins.
 *
 * This action allows the Merchant to add a coin to his total, but only if he has 3 or more coins.
 * It deducts 1 coin from the bank and adds it to the Merchant's total.
 *
 * @param None
 * @return void
 * @throws None
 */
void Merchant::addCoin()
{
    if (owner->getCoins() >= 3)
    {
        // If the Merchant has 3 or more coins, he can add a coin to his total
        game->changeCoinsInBank(-1); // Deduct 1 coin from the bank
        owner->addNumCoins(1);
        cout << owner->getName() << " has added 1 coin to his total." << endl;
        game->nextTurn(); // Move to the next player's turn
        return;
    }
}

/**
 * @brief This function delegates the pure virtual function Action() of the Character class to the addCoin() method of the Merchant.
 *
 * @param None
 * @return void
 * @throws None
 */
void Merchant::Action()
{
        addCoin();
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
void Merchant::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Add coin" << endl;
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
        Action(); // Call the Action method for add coin
        break;
    case 8:
        // Skip turn action
        game->nextTurn(); // Move to the next player's turn
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
        chooseAction(); // Retry if invalid
        break;
    }
}
