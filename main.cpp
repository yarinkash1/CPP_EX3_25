// yarinkash1@gmail.com
#include <iostream>
#include "Game.hpp"
#include "Player.hpp" // Include the header file for the Player class
#include "Character.hpp" // Include the header file for the Character class
#include "Baron.hpp"     // Include the header file for the Baron class

int main()
{
    Player player1("Player 1", nullptr, 10);
    Baron baron(&player1);
    baron.Action(1); // Call the invest action

    return 0;
}
