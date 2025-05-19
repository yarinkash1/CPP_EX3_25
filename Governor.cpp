#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"
#include "Game.hpp"

Governor::Governor(Player* p, Game* g) : Character(p, g){}

void Governor::tax()
{
    game->changeCoinsInBank(-2); // Deduct 2 coins from the bank
    owner->addNumCoins(2); // Add 2 coins to the player's total
    std::cout << owner->getName() << " has taken 2 coins from the bank." << std::endl;
}
void Governor::cancelTax(Player &target)
{
    target.setIsTaxPrevented(true); // Set the target player as tax prevented
    cout << owner->getName() << " has canceled the tax on " << target.getName() << std::endl;
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
        std::cout << "Invalid action type for Governor." << std::endl;
        break;
    }
}