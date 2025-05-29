// yarinkash1@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include "Game.hpp"
#include "Player.hpp"
#include "Character.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"



// Test functions that normally use GUI
TEST_CASE("Target Player Selection Logic") 
{
    vector<string> playerNames = {"Player1", "Player2", "Player3"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Test filtering logic for arrest action
    vector<Player*> validTargets;
    for (Player* player : players) {
        if (player->getId() != 1) { // Not current player
            if (!player->getIsArrested() && player->getCoins() >= 1) {
                validTargets.push_back(player);
            }
        }
    }
    
    CHECK(validTargets.size() == 2); // Two other players
    
    game.resetInstance();
}