#include "Player.hpp"
#include "Character.hpp"
#include "Governor.hpp"

Governor::Governor(Player *player)
{
    this->player = player;
}

