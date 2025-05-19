#include "Player.hpp"
#include "Character.hpp"
#include "General.hpp"
#include "Game.hpp"

General::General(Player* p, Game* g) : Character(p, g){}

void General::preventCoup(Player &target)
{
    owner->removeNumCoins(5); // Remove 5 coins from the owner
    game->changeCoinsInBank(+5); // Add 5 coins to the bank
    target.setIsCoupPrevented(true); // Set the target player as coup prevented
    cout << owner->getName() << " has prevented a coup on " << target.getName() << std::endl;
    arrestCoinCompensation(); // Call the arrest coin compensation method
}
void General::arrestCoinCompensation()
{
    if(owner->getIsArrested() == 1) // 1 = True
    {
        game->changeCoinsInBank(-1);
        owner->addNumCoins(1);

        cout << owner->getName() << " has received 1 coin as compensation for being arrested." << endl;
    }
}

void General::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for preventCoup
        preventCoup(*owner);
        break;
    case 2: // Delegate action type for arrestCoinCompensation
        arrestCoinCompensation();
        break;
    default:
        std::cout << "Invalid action type for General." << std::endl;
        break;
    }
}