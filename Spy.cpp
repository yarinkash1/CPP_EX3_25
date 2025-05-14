#include "Player.hpp"
#include "Character.hpp"
#include "Spy.hpp"

Spy::Spy(Player* p) : Character(p) {}

void Spy::peekAndPreventArrest(Player &Player)
{
    // Implementation of the peekAndPreventArrest method
    // This method is specific to the Spy character and handles the peek and prevent arrest action
}


void Spy::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for peekAndPreventArrest
        peekAndPreventArrest(*player);
        break;
    default:
        std::cout << "Invalid action type for Spy." << std::endl;
        break;
    }
}
