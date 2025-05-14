#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"

Governor::Governor(Player* p) : Character(p) {}

void Governor::tax()
{
    // Implementation of the tax method
}
void Governor::cancelTax(Player &Player)
{
    // Implementation of the cancelTax method
    // This method is specific to the Governor character and handles the cancel tax action
}

void Governor::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for tax
        tax();
        break;
    case 2: // Delegate action type for cancelTax
        cancelTax(*player);
        break;
    default:
        std::cout << "Invalid action type for Governor." << std::endl;
        break;
    }
}