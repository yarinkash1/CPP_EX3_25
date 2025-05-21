// yarinkash1@gmail.com

#include "Player.hpp"
#include "Character.hpp"
#include "General.hpp"
#include "Game.hpp"

General::General(Player* p, Game* g) : Character(p, g){}

void General::preventCoup(Player &target)
{
    owner->removeNumCoins(5); // Remove 5 coins from the owner
    game->changeCoinsInBank(+5); // Add 5 coins to the bank
    target.setIsCoupPrevented(true); // Set the target player as coup prevented
    cout << owner->getName() << " has prevented a coup on " << target.getName() << std::endl;
    arrestCoinCompensation(); // Call the arrest coin compensation method
}
void General::arrestCoinCompensation()
{
    if(owner->getIsArrested() == 1) // 1 = True
    {
        game->changeCoinsInBank(-1);
        owner->addNumCoins(1);

        cout << owner->getName() << " has received 1 coin as compensation for being arrested." << endl;
    }
}

void General::Action(int actionType)
{
    switch (actionType)
    {
    case 1: // Delegate action type for preventCoup
        preventCoup(*owner);
        break;
    case 2: // Delegate action type for arrestCoinCompensation
        arrestCoinCompensation();
        break;
    default:
        std::cout << "Invalid action type for General." << std::endl;
        break;
    }
}

void General::chooseAction()
{
    int actionType;
    cout << "Choose an action: " << endl;
    cout << "1. Gather" << endl;
    cout << "2. Tax" << endl;
    cout << "3. Bribe" << endl;
    cout << "4. Arrest" << endl;
    cout << "5. Sanction" << endl;
    cout << "6. Coup" << endl;
    cout << "7. Prevent Coup" << endl;
    cout << "8. Arrest coin compensation" << endl;
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
            Action(1); // Call the Action method for Prevent Coup
            break;
        case 8:
            Action(2); // Call the Action method for arrest coin compensation
            break;
        case 9:
            cout << "Turn skipped." << endl;
            break;
        default:
            cout << "Invalid action type. Please try again." << endl;
    }
}