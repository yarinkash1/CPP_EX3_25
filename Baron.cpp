// yarinkash1@gmail.com

#include "Baron.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Game.hpp"

Baron::Baron(Player* p, Game* g) : Character(p, g){}

void Baron::invest()
{
    if(owner->getCoins() >= 3)
    {
        // Deduct 3 coins from the player:
        owner->setCoins(owner->getCoins() -3);
        game->changeCoinsInBank(+3); // Add 3 coins to the bank

        // Add 6 coins to the player:
        game->changeCoinsInBank(-6);// deduct 6 coins from the bank
        owner->setCoins(owner->getCoins() + 6);

        cout << owner->getName() << " has invested 3 coins and received 6 coins." << endl;

        sanctionCoinCompensation(); // Call the sanction coin compensation method
    }
    else
    {
        cout << owner->getName() << " does not have enough coins to invest." << endl;
    }
}
void Baron::sanctionCoinCompensation()
{
    if(owner->getIsSanctioned() == 1) // 1 = True
    {
        game->changeCoinsInBank(-1);
        owner->addNumCoins(1);

        cout << owner->getName() << " has received 1 coin as compensation for being sanctioned." << endl;
    }
}
void Baron::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for invest
        invest();
        break;
    case 2: // Delegate action type for sanction coin compensation
        sanctionCoinCompensation();
        break;
    default:
        cout << "Invalid action type for Baron." << endl;
        break;
    }
}

void Baron::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Invest" << endl;
    cout << "8. Sanction coin compensation" << endl;
    cout << "9. Skip turn" << endl;
    cin >> actionType;

    if (actionType < 1 || actionType > 9)
    {
        cout << "Invalid action type. Please try again." << endl;
        chooseAction(); // Retry if invalid
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
            Action(1); // Call the Action method for invest
            break;
        case 8:
            Action(2); // Call the Action method for sanction coin compensation
            break;
        case 9:
            cout << "Turn skipped." << endl;
            break;
        default:
            cout << "Invalid action type. Please try again." << endl;
    }

}

