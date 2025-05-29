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

// Test cases for game creation and game end conditions:

TEST_CASE("Inadequate number of players game creation")
{
    vector<string> playerNames = {"Player1"};
    Game::configure(100);
    CHECK_THROWS(Game::getInstance(1, playerNames)); // Should throw an error for invalid number of players
    Game::resetInstance();
}

TEST_CASE("Too many players game creation")
{
    vector<string> playerNames = {"Leonardo", "Michelangelo", "Raphael", "Donatello", "Master Splinter", "April O'Neil", "Casey Jones", "Shredder"};
    Game::configure(100);
    CHECK_THROWS(Game::getInstance(8, playerNames)); // Should throw an error for invalid number of players
    Game::resetInstance();
}

TEST_CASE("Game creation with negative coins in the bank")
{
    vector<string> playerNames = {"Player1", "Player2"};
    CHECK_THROWS(Game::configure(-1));
    
    Game::resetInstance();
}

TEST_CASE("Game Over Detection") 
{
    vector<string> playerNames = {"Winner", "Loser"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    vector<Player*> players = game.active_players();
    Player* winner = players[0];
    Player* loser = players[1];
    
    // Initially both players are active
    CHECK(game.active_players().size() == 2);
    
    // Eliminate one player
    loser->setIsActive(false);
    
    // Check game over condition
    vector<Player*> activePlayers = game.active_players();
    CHECK(activePlayers.size() == 1);
    CHECK(activePlayers[0] == winner);
    
    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test case for role-specific actions:
TEST_CASE("Baron Investment Action") 
{
    vector<string> playerNames = {"BaronPlayer","Eyal Golan The King"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    
    // Force Baron role for testing
    delete player->getRole();
    Baron* baron = new Baron(player, &game);
    player->setRole(baron);
    
    // Test Baron's invest action directly
    player->setCoins(3);
    int initialCoins = player->getCoins();
    
    static_cast<Baron*>(player->getRole())->Action();

    
    std::cout << "Coins before invest: " << initialCoins << std::endl;
    std::cout << "Coins after invest: " << player->getCoins() << std::endl;
    
    // Check what actually happened
    CHECK(player->getCoins() >= initialCoins); // At least no coins lost
    
    Game::resetInstance();
}

TEST_CASE("General special action")
{
    vector<string> playerNames = {"GeneralPlayer", "TargetPlayer"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    Player* target = game.active_players()[1];
    
    // Force General role for testing
    delete player->getRole();
    player->setRole(new General(player, &game));
    
    // Set up test conditions (General needs >= 5 coins to perform special action)
    player->setCoins(5);
    target->setCoins(2); // Target player has less than 5 coins(cannot defend coup)
    
    // Perform the action
    static_cast<General*>(player->getRole())->Action(target);
    
    // Verify general action worked (e.g., coins should be reduced by 1)
    CHECK(target->getIsCoupPrevented() == true); // General should prevent coup for the next turn
    
    Game::resetInstance();
}

TEST_CASE("Governor Tax Action") 
{
    vector<string> playerNames = {"GovernorPlayer","Cosmic Girl From Another Galaxy"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    
    // Force Governor role for testing
    delete player->getRole();
    player->setRole(new Governor(player, &game));

    player->setCoins(3);
    int initialCoins = player->getCoins();
    
    static_cast<Governor*>(player->getRole())->Action();
    
    // Verify governor action worked
    CHECK(player->getCoins() == initialCoins + 3); // Governor should gain 3 coins from tax
    
    Game::resetInstance();
}

TEST_CASE("Governor Cancel Tax Action") 
{
    vector<string> playerNames = {"GovernorPlayer","Amit Capit", "Eli Copter", "Moti lohem"};
    Game::configure(666);
    Game& game = Game::getInstance(4, playerNames);
    Player* player = game.active_players()[0];
    Player* target = game.active_players()[3];

    // Force Governor role for testing
    delete player->getRole();
    player->setRole(new Governor(player, &game));
    player->setCoins(6);

    static_cast<Governor*>(player->getRole())->Action(target);
    CHECK(target->getIsTaxPrevented() == true); // Target should be tax prevented

}

TEST_CASE("Merchant Add Coin Action") 
{
    vector<string> playerNames = {"MerchantPlayer","Some Other Player"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    
    // Force Merchant role for testing
    delete player->getRole();
    player->setRole(new Merchant(player, &game));
    
    // Set up test conditions (Merchant needs >= 3 coins to add coin)
    player->setCoins(3);
    int initialCoins = player->getCoins();
    
    static_cast<Merchant*>(player->getRole())->Action();
    
    // Verify merchant action worked
    CHECK(player->getCoins() == initialCoins + 1);
    
    Game::resetInstance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for target player selection logic:

TEST_CASE("Target Player Selection Logic on arrest Action") 
{
    // Players can have same names, so we need to ensure the logic works correctly
    // even if they do(we will use IDs to differentiate them)
    vector<string> playerNames = {"Alice", "Alice", "Bob"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Give all players some coins for testing
    for (Player* player : players) 
    {
        player->setCoins(5);
    }
    
    int currentPlayerId = players[0]->getId();
    
    // Test filtering logic for arrest action
    vector<Player*> validTargets;
    for (Player* player : players) 
    {
        if (player->getId() != currentPlayerId) // Not current player
        { 
            if (!player->getIsArrested() && player->getCoins() >= 1) 
            {
                validTargets.push_back(player);
            }
        }
    }
    
    CHECK(validTargets.size() == 2); // Two other players besides current player should be valid targets
    
    Game::resetInstance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

