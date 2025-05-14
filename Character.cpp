#include "Player.hpp"
#include "Character.hpp"


void Character::gather()
{
    // Implementation of the gather method
    owner->setCoins(owner->getCoins() + 1); // Increase coins by 1
    std::cout << "Gather action performed." << std::endl;
}

void Character::tax()
{
    // Implementation of the tax method
    std::cout << "Tax action performed." << std::endl;
}

void Character::bribe()
{
    // Implementation of the bribe method
    std::cout << "Bribe action performed." << std::endl;
}

void Character::arrest(Player &player)
{
    // Implementation of the arrest method
    std::cout << "Arrest action performed on player: " << player.getName() << std::endl;
}

void Character::sanction(Player &player)
{
    // Implementation of the sanction method
    std::cout << "Sanction action performed on player: " << player.getName() << std::endl;
}

void Character::coup(Player &player)
{
    // Implementation of the coup method
    std::cout << "Coup action performed on player: " << player.getName() << std::endl;
}
