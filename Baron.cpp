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
        owner->setCoins(owner->getCoins() + 6);

        game->changeCoinsInBank(-6);// deduct 6 coins from the bank

        std::cout << owner->getName() << " has invested 3 coins and received 6 coins." << std::endl;
    }
    else
    {
        std::cout << owner->getName() << " does not have enough coins to invest." << std::endl;
    }
}
void Baron::sanctionCoinCompensation()
{
    // Implementation of the sanctionCoinCompensation method
    // This method is specific to the Baron character and handles the sanction coin compensation action
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
        std::cout << "Invalid action type for Baron." << std::endl;
        break;
    }
}
