// yarinkash1@gmail.com

#include "Baron.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Game.hpp"

Baron::Baron(Player* p, Game* g) : Character(p, g){}

/**
 * @brief Invests 3 coins to receive 6 coins.
 *
 * This action allows the Baron to invest 3 coins and receive 6 coins in return.
 * It checks if the player has enough coins before proceeding with the investment.
 *
 * @param None
 * @return void
 * @throws None
 */
void Baron::invest()
{
    if(owner->getCoins() >= 3)
    {
        // Deduct 3 coins from the player:
        owner->setCoins(owner->getCoins() -3);
        game->changeCoinsInBank(+3); // Add 3 coins to the bank

        // Add 6 coins to the player:
        game->changeCoinsInBank(-6);// deduct 6 coins from the bank
        owner->setCoins(owner->getCoins() + 6);

        cout << owner->getName() << " has invested 3 coins and received 6 coins." << endl;
        game->nextTurn(); // Move to the next player's turn
        return;
    }
    else
    {
        cout << owner->getName() << " does not have enough coins to invest." << endl;
        chooseAction(); // Prompt the player to choose another action
    }

}

/**
 * @brief This funtion delegates the pure virtual function Action() of the Character class to the invest() method of the Baron.
 *
 * @param None
 * @return void
 * @throws None
 */
void Baron::Action()
{
    invest();
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
void Baron::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Invest" << endl;
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
            Action(); // Call the Action method for invest
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

