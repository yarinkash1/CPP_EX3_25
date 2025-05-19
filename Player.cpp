#include "Player.hpp"

void Player::removeNumCoins(int num_coins)
{
    if (coins >= num_coins)
    {
        coins -= num_coins;
    }
    else
    {
        std::cout << "Not enough coins to remove." << std::endl;
    }
}

int Player::next_id = 1;
