#include "Player.hpp"
#include "Character.hpp"
#include "Judge.hpp"
#include "Game.hpp"

Judge::Judge(Player* p, Game* g) : Character(p, g){}

void Judge::cancelBribe(Player &target)
{
    target.setIsBribePrevented(true); // Set the target player as bribe prevented
    cout << owner->getName() << " has canceled the bribe on " << target.getName() << std::endl;
}

void Judge::coinForTheBankOverSanction(Player &target)
{
    if(owner->getIsSanctioned()==1)
    {
        target.removeNumCoins(1); // Remove 1 coin from the target player
        game->changeCoinsInBank(1); // Add 1 coin to the bank
        cout << target.getName() << " Was deducted 1 coin for sacntioning a Judge" << std::endl;
        
    }
}

void Judge::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for cancelBribe
        cancelBribe(*owner);
        break;
    case 2: // Delegate action type for coinForTheBankOverSanction
        coinForTheBankOverSanction(*owner);
        break;
    default:
        std::cout << "Invalid action type for Judge." << std::endl;
        break;
    }
}
