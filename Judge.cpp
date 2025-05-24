// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "Judge.hpp"
#include "Game.hpp"

Judge::Judge(Player* p, Game* g) : Character(p, g){}

void Judge::cancelBribe(Player &target)
{
    target.setIsBribePrevented(true); // Set the target player as bribe prevented
    cout << owner->getName() << " has canceled the bribe on " << target.getName() << endl;
}


void Judge::Action()
{
    cancelBribe(*owner);
}

void Judge::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Cancel Bribe" << endl;
    cout << "8. Skip turn" << endl;
    cin >> actionType;

    if (actionType < 1 || actionType > 8)
    {
        cout << "Invalid action type. Please try again." << endl;
        return;
    }

    switch (actionType)
    {
    case 1:
        this->gather();
        break;
    case 2:
        this->tax();
        break;
    case 3:
        this->bribe();
        break;
    case 4:
        this->arrest();
        break;
    case 5:
        this->sanction();
        break;
    case 6:
        this->coup();
        break;
    case 7:
        Action(); // Call the Action method for cancel bribe
        break;
    case 8:
        // Skip turn logic
        game->nextTurn(); // Move to the next player's turn
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
    }
}
