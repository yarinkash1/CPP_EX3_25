// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Merchant.hpp"
#include "Game.hpp"

Merchant::Merchant(Player* p, Game* g) : Character(p, g){}

void Merchant::addCoin()
{
    if (owner->getCoins() >= 3)
    {
        // If the Merchant has 3 or more coins, he can add a coin to his total
        game->changeCoinsInBank(-1); // Deduct 1 coin from the bank
        owner->addNumCoins(1);
        cout << owner->getName() << " has added 1 coin to his total." << endl;
    }
}

void Merchant::arrestAlternate()
{
    if(owner->getIsArrested() == 1)
    {
        // If the Merchant is arrested, he pays 2 coins to the bank instead of losing 1 coin to another player
        owner->removeNumCoins(2); // Remove 2 coins from the Merchant
        game->changeCoinsInBank(+2); // Add 2 coins to the bank

        cout << owner->getName() << " has paid 2 coins to the bank due to arrest." << endl;
    }
}

void Merchant::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for addCoin
        addCoin();
        break;
    case 2: // Delegate action type for arrestAlternate
        arrestAlternate();
        break;
    default:
        cout << "Invalid action type for Merchant." << endl;
        break;
    }
}

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
    cout << "8. Arrest Alternate" << endl;
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
        Action(1); // Call the Action method for add coin
        break;
    case 8:
        Action(2); // Call the Action method for arrest alternate
        break;
    case 9:
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
    }
}
