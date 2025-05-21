// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"
#include "Game.hpp"

Governor::Governor(Player *p, Game *g) : Character(p, g) {}

void Governor::tax()
{
    game->changeCoinsInBank(-2); // Deduct 2 coins from the bank
    owner->addNumCoins(2);       // Add 2 coins to the player's total
    cout << owner->getName() << " has taken 2 coins from the bank." << endl;
}
void Governor::cancelTax(Player &target)
{
    target.setIsTaxPrevented(true); // Set the target player as tax prevented
    cout << owner->getName() << " has canceled the tax on " << target.getName() << endl;
}

void Governor::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for tax
        tax();
        break;
    case 2: // Delegate action type for cancelTax
        cancelTax(*owner);
        break;
    default:
        cout << "Invalid action type for Governor." << endl;
        break;
    }
}

void Governor::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Governor Tax" << endl;
    cout << "8. Cancel Tax" << endl;
    cout << "9. Skip turn" << endl;
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
        Action(1); // Call the Action method for Governor Tax
        break;
    case 8:
        Action(2); // Call the Action method for tax cancellation
        break;
    case 9:
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
    }
}