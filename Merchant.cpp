#include "Player.hpp"
#include "Character.hpp"
#include "Merchant.hpp"

Merchant::Merchant(Player* p, Game* g) : Character(p, g), player(p) {}

void Merchant::addCoin()
{
    // Implementation of the addCoin method
    // This method is specific to the Merchant character and handles the coin addition action
}

void Merchant::arrestAlternate()
{
    // Implementation of the arrestAlternate method
    // This method is specific to the Merchant character and handles the alternate arrest action
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
        std::cout << "Invalid action type for Merchant." << std::endl;
        break;
    }
}
