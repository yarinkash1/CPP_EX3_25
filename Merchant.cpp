#include "Player.hpp"
#include "Character.hpp"
#include "Merchant.hpp"

Merchant::Merchant(Player *player)
{
    this->player = player;
}
