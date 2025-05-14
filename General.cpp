#include "Player.hpp"
#include "Character.hpp"
#include "General.hpp"

General::General(Player* p) : Character(p) {}

void General::preventCoup(Player &Player)
{
    // Implementation of the preventCoup method
    // This method is specific to the General character and handles the coup prevention action
}
void General::arrestCoinCompensation()
{
    // Implementation of the arrestCoinCompensation method
    // This method is specific to the General character and handles the arrest coin compensation action
}


void General::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for preventCoup
        preventCoup(*player);
        break;
    case 2: // Delegate action type for arrestCoinCompensation
        arrestCoinCompensation();
        break;
    default:
        std::cout << "Invalid action type for General." << std::endl;
        break;
    }
}