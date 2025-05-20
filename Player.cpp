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

int Player::next_id = 1;
