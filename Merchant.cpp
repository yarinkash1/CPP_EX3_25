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
