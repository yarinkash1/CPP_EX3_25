#include "Baron.hpp"
#include "Character.hpp"

class Player; // forward declaration


Baron::Baron(Player *player)
{
    this->player = player;
}
void Baron::invest()
{
    if(player->getCoins() >= 3)
    {
        player->setCoins(player->getCoins() -3);
        // add 3 coins to the bank

        player->setCoins(player->getCoins() + 6);
        // deduct 3 coins from the bank
        std::cout << player->getName() << " has invested 3 coins and received 6 coins." << std::endl;
    }
    else
    {
        std::cout << player->getName() << " does not have enough coins to invest." << std::endl;
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
