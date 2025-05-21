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

void Judge::coinForTheBankOverSanction(Player &target)
{
    if(owner->getIsSanctioned()==1)
    {
        target.removeNumCoins(1); // Remove 1 coin from the target player
        game->changeCoinsInBank(1); // Add 1 coin to the bank
        cout << target.getName() << " Was deducted 1 coin for sacntioning a Judge" << endl;
        
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
        cout << "Invalid action type for Judge." << endl;
        break;
    }
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
    cout << "8. Coin for the bank" << endl;
    cout << "9. Skip turn" << endl;
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
        Action(1); // Call the Action method for cancel bribe
        break;
    case 8:
        Action(2); // Call the Action method for coin for the bank
        break;
    case 9:
        cout << "Turn skipped." << endl;
        break;
    default:
        cout << "Invalid action type. Please try again." << endl;
    }
}
