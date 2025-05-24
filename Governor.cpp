// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"
#include "Game.hpp"

Governor::Governor(Player *p, Game *g) : Character(p, g) {}

void Governor::tax()
{
    game->changeCoinsInBank(-3); // Deduct 3 coins from the bank
    owner->addNumCoins(3);       // Add 3 coins to the player's total
    cout << owner->getName() << " has taken 3 coins from the bank." << endl;
}
void Governor::cancelTax(Player &target)
{
    target.setIsTaxPrevented(true); // Set the target player as tax prevented
    cout << owner->getName() << " has canceled the tax on " << target.getName() << endl;
}

void Governor::Action()
{
    cancelTax(*owner);
}

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
    }
}