// yarinkash1@gmail.com

#include "Player.hpp"

/**
 * @brief Remove a certain amount of coins from the player
 *
 * @param num_coins The coins amount
 * @return void
 * @throws runtime_error if there are not enough coins to remove
 */
void Player::removeNumCoins(int num_coins)
{
    if (coins >= num_coins)
    {
        coins -= num_coins;
    }
    else
    {
        throw runtime_error("Not enough coins to remove.");
    }
}

/**
 * @brief Print the current status of a player
 * 
 * @note This function is obsolete because the GUI handles the player status display.
 * It is kept for reference and debugging purposes.
 * @param none
 * @return void
 * @throws none
 */
void Player::printPlayerInfo()
{
    cout << "Player ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Coins: " << coins << endl;
    cout << "Is Active: " << (is_active ? "Yes" : "No") << endl;
    cout << "Role: " << (role ? role->getRoleName() : "None") << endl;
    cout << "Is Sanctioned: " << (isSanctioned ? "Yes" : "No") << endl;
    cout << "Is Peeked and Arrest Prevented: " << (isPeekedAndArrestPrevented ? "Yes" : "No") << endl;
    cout << "Is Coup Prevented: " << (isCoupPrevented ? "Yes" : "No") << endl;
    cout << "Is Tax Prevented: " << (isTaxPrevented ? "Yes" : "No") << endl;
    cout << "Is Bribe Prevented: " << (isBribePrevented ? "Yes" : "No") << endl;
    cout << "Is Arrested: " << (isArrested ? "Yes" : "No") << endl;
    cout << "Its turn: " << (is_turn == true ? "Yes" : "No") << endl;
    cout << "----------------------------------------------------------------------------------------------------------------" << endl;
}

// Static member initialization
int Player::next_id = 1;