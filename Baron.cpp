#include "Baron.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Game.hpp"

Baron::Baron(Player* p, Game* g) : Character(p, g){}

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

        sanctionCoinCompensation(); // Call the sanction coin compensation method
    }
    else
    {
        cout << owner->getName() << " does not have enough coins to invest." << endl;
    }
}
void Baron::sanctionCoinCompensation()
{
    if(owner->getIsSanctioned() == 1) // 1 = True
    {
        game->changeCoinsInBank(-1);
        owner->addNumCoins(1);

        cout << owner->getName() << " has received 1 coin as compensation for being sanctioned." << endl;
    }
}
void Baron::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for invest
        invest();
        break;
    case 2: // Delegate action type for sanction coin compensation
        sanctionCoinCompensation();
        break;
    default:
        cout << "Invalid action type for Baron." << endl;
        break;
    }
}
