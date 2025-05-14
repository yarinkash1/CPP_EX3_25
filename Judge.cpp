#include "Player.hpp"
#include "Character.hpp"
#include "Judge.hpp"

Judge::Judge(Player* p) : Character(p) {}

void Judge::cancelBribe(Player &Player)
{
    // Implementation of the cancelBribe method
    // This method is specific to the Judge character and handles the cancel bribe action
}
void Judge::coinForTheBankOverSanction(Player &Player)
{
    // Implementation of the coinForTheBankOverSanction method
    // This method is specific to the Judge character and handles the coin for the bank over sanction action
}

void Judge::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for cancelBribe
        cancelBribe(*player);
        break;
    case 2: // Delegate action type for coinForTheBankOverSanction
        coinForTheBankOverSanction(*player);
        break;
    default:
        std::cout << "Invalid action type for Judge." << std::endl;
        break;
    }
}
