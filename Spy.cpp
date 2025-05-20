#include "Player.hpp"
#include "Character.hpp"
#include "Spy.hpp"
#include "Game.hpp"

Spy::Spy(Player* p, Game* g) : Character(p, g){}

void Spy::peekAndPreventArrest(Player &target)
{
    int target_num_coins = target.getCoins(); // Get the number of coins of the target player
    cout << target.getName() << " has " << target_num_coins << " coins." << endl;
    target.setIsArrestPrevented(true); // Set the target player as arrest prevented
    cout << owner->getName() << " has peeked at " << target.getName() << "'s coins and prevented their arrest for their next turn." << endl;
}

void Spy::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for peekAndPreventArrest
        peekAndPreventArrest(*owner);
        break;
    default:
        cout << "Invalid action type for Spy." << endl;
        break;
    }
}
