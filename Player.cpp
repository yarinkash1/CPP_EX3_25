// yarinkash1@gmail.com

#include "Player.hpp"

void Player::removeNumCoins(int num_coins)
{
    if (coins >= num_coins)
    {
        coins -= num_coins;
    }
    else
    {
        cout << "Not enough coins to remove." << endl;
    }
}

void Player::printPlayerInfo()
{
    cout << "Player ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Coins: " << coins << endl;
    cout << "Is Active: " << (is_active ? "Yes" : "No") << endl;
    cout << "Win Counter: " << win_counter << endl;
    cout << "Role: " << (role ? role->getRoleName() : "None") << endl;
    cout << "Is Arrested: " << (isArrested ? "Yes" : "No") << endl;
    cout << "Is Sanctioned: " << (isSanctioned ? "Yes" : "No") << endl;
    cout << "Is Peeked and Arrest Prevented: " << (isPeekedAndArrestPrevented ? "Yes" : "No") << endl;
    cout << "Is Coup Prevented: " << (isCoupPrevented ? "Yes" : "No") << endl;
    cout << "Is Tax Prevented: " << (isTaxPrevented ? "Yes" : "No") << endl;
    cout << "Is Bribe Prevented: " << (isBribePrevented ? "Yes" : "No") << endl;
    cout << "Is Arrest Prevented: " << (isArrestPrevented ? "Yes" : "No") << endl;
    cout << "Its turn: " << (is_turn == true ? "Yes" : "No") << endl;
    cout << "----------------------------------------------------------------------------------------------------------------" << endl;
}


int Player::next_id = 1;
