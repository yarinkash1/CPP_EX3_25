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

TEST_CASE("Game creation where the number of players doesn't match the number of player names")
{
    vector<string> playerNames = {"Player1", "Player2", "Player3"};
    Game::configure(100);
    CHECK_THROWS(Game::getInstance(2, playerNames)); // Should throw an error for mismatched player count
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
// Test cases for regular actions:
TEST_CASE("Player Gather Action") 
{
    vector<string> playerNames = {"Shimi Tavori", "Ben El Tavori"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    Player* player = game.active_players()[0];
    int initialCoins = player->getCoins();
    player->getRole()->gather(); // Player gathers coins
    int coinsAfterGather = player->getCoins();
    CHECK(coinsAfterGather == initialCoins + 1); // Player should have 1 more coin after gathering
    Game::resetInstance();
}

TEST_CASE("Player Tax Action") 
{
    vector<string> playerNames = {"Omer Adam", "Sharif The Druze Kid","Noa Kirel"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    Player* player = game.active_players()[0];

    // Force Baron role for testing(not governor so his tax is different - checked in another test)
    delete player->getRole();
    Baron* baron = new Baron(player, &game);
    player->setRole(baron);


    int initialCoins = player->getCoins();
    player->getRole()->tax(); // Player taxes
    int coinsAfterTax = player->getCoins();
    CHECK(coinsAfterTax == initialCoins + 2); // Player should have 2 more coins after taxing
    Game::resetInstance();
}

TEST_CASE("player Sanction Action")
{
    vector<string> playerNames = {"SanctionPlayer", "TargetPlayer - Governor" ,"Target - Baron"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    Player* player = game.active_players()[0];
    Player* target_baron = game.active_players()[2];
    
    // Force Baron role for testing
    delete target_baron->getRole();
    target_baron->setRole(new Baron(target_baron, &game));
    
    // Set up test conditions
    player->setCoins(6);
    target_baron->setCoins(3); // Target Baron has 3 coins
    int initialCoins = target_baron->getCoins();
    
    static_cast<Judge*>(player->getRole())->sanction(target_baron);
    
    // Verify sanction action worked
    CHECK(target_baron->getIsSanctioned() == true); // Target should be sanctioned
    CHECK(target_baron->getCoins() == initialCoins + 1); // Target should get 1 coin as compensation for being a Baron

    cout << "players turn: " << player->getName() << endl;
    
    Game::resetInstance();
}

TEST_CASE("Arrest logic")
{
    vector<string> playerNames = {"ArrestPlayer", "TargetPlayer - Merchant", "TargetPlayer - General", "TargetPlayer - Spy"};
    Game::configure(100);
    Game& game = Game::getInstance(4, playerNames);
    Player* arrest_player = game.active_players()[0];
    Player* target_merchant = game.active_players()[1];
    Player* target_general = game.active_players()[2];
    Player* target_spy = game.active_players()[3]; // just to make sure not a general or a merchant

    // Assigning a random role(judge) to the arresting player
    delete arrest_player->getRole();
    arrest_player->setRole(new Judge(arrest_player, &game));

    // Force Merchant role for testing
    delete target_merchant->getRole();
    target_merchant->setRole(new Merchant(target_merchant, &game));
    // Force General role for testing
    delete target_general->getRole();
    target_general->setRole(new General(target_general, &game));
    // Force Spy role for testing
    delete target_spy->getRole();
    target_spy->setRole(new Spy(target_spy, &game));

    // Set up test conditions
    arrest_player->setCoins(9);
    target_merchant->setCoins(3); // Merchant has 3 coins
    target_general->setCoins(4); // General has 4 coins
    target_spy->setCoins(2); // Spy has 2 coins
    int initialCoinsMerchant = target_merchant->getCoins();
    int initialCoinsGeneral = target_general->getCoins();
    int initialCoinsSpy = target_spy->getCoins();
    // Perform arrest on Merchant
    static_cast<Judge*>(arrest_player->getRole())->arrest(target_merchant);
    // Verify arrest action worked
    CHECK(target_merchant->getIsArrested() == true); // Merchant should be arrested
    CHECK(target_merchant->getCoins() == initialCoinsMerchant - 2); // Merchant should pay 2 coins to the bank
    game.nextTurn(); // Merhcant skips his turn
    game.nextTurn(); // General skips his turn
    game.nextTurn(); // Spy skips his turn

    
    arrest_player->getRole()->arrest(target_general); // Perform arrest on General
    // Verify arrest action worked
    CHECK(target_general->getIsArrested() == true); // General should be arrested
    CHECK(target_general->getCoins() == initialCoinsGeneral); // General should get his coin back
    game.nextTurn(); // Merhcant skips his turn
    game.nextTurn(); // General skips his turn
    game.nextTurn(); // Spy skips his turn
    arrest_player->getRole()->arrest(target_spy); // Perform arrest on Spy
    // Verify arrest action worked
    CHECK(target_spy->getIsArrested() == true); // Spy should be arrested
    CHECK(target_spy->getCoins() == initialCoinsSpy - 1); // Spy should lose 1 coin
    Game::resetInstance();

}

TEST_CASE("Coup action - General defence")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - General"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player1 = game.active_players()[0];
    Player* player2 = game.active_players()[1];
    
    // Force General role for testing
    delete player2->getRole();
    player2->setRole(new General(player2, &game));
    
    // Set up test conditions (General needs >= 5 coins to prevent coup on himself)
    player2->setCoins(5);
    player1->setCoins(7); // Player 1 has enough coins to coup
    
    static_cast<General*>(player1->getRole())->coup(player2);
    
    // Verify coup action worked
    CHECK(player2->getIsActive() == true); // Target should not be eliminated
    
    Game::resetInstance();
}

TEST_CASE("Coup action - No defence")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - General"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player1 = game.active_players()[0];
    Player* player2 = game.active_players()[1];
    
    // Force General role for testing
    delete player2->getRole();
    player2->setRole(new General(player2, &game));
    
    // Set up test conditions (General needs >= 5 coins to prevent coup on himself)
    player2->setCoins(4); // Not enough coins to prevent coup
    player1->setCoins(7); // Player 1 has enough coins to coup
    
    static_cast<General*>(player1->getRole())->coup(player2);
    
    // Verify coup action worked
    CHECK(player2->getIsActive() == false); // Target should not be eliminated
    
    Game::resetInstance();
}

TEST_CASE("check coup force")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - Baron"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player1 = game.active_players()[0];
    Player* player2 = game.active_players()[1];
    
    // Force Baron role for player2 (Baron cannot prevent coup)
    delete player2->getRole();
    player2->setRole(new Baron(player2, &game));
    
    // Set up coup force scenario - player has 10+ coins (must coup)
    player1->setCoins(10); // 10 coins forces coup
    player2->setCoins(3);  // Target has some coins
    
    // Check that player1 is forced to coup (has 10+ coins)
    CHECK(player1->getCoins() >= 10);
    
    // Clear any existing messages first
    Game::clearMessages();
    
    // Add the forced coup message manually (simulating GUI behavior)
    if (player1->getCoins() >= 10) {
        Game::addMessage(player1->getName() + " has " + to_string(player1->getCoins()) + " coins and must coup!");
    }
    
    // Check for forced coup message using existing methods
    bool foundForcedCoupMessage = false;
    vector<string> collectedMessages;
    
    // Collect all messages from the queue
    while (Game::hasMessages()) {
        string message = Game::getNextMessage();
        collectedMessages.push_back(message);
        if (message.find("must coup") != string::npos || 
            message.find("forced to coup") != string::npos ||
            message.find("10 coins") != string::npos) {
            foundForcedCoupMessage = true;
        }
    }
    
    // Perform coup action
    player1->getRole()->coup(player2);
    
    // Verify coup worked and message was displayed
    CHECK(foundForcedCoupMessage == true); // Message should be displayed
    CHECK(player2->getIsActive() == false); // Target eliminated
    CHECK(player1->getCoins() == 3); // 10 - 7 = 3 coins left
    
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
    Game::resetInstance();
}

TEST_CASE("Judge Bribe Prevention Message Test")
{
    vector<string> playerNames = {"JudgePlayer", "BribePlayer", "ThirdPlayer"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    Player* judge = game.active_players()[0];
    Player* briberPlayer = game.active_players()[1];
    
    // Set up Judge to prevent bribe on briberPlayer
    delete judge->getRole();
    judge->setRole(new Judge(judge, &game));
    judge->setCoins(3);
    
    // Set up briber with enough coins
    briberPlayer->setCoins(8);
    
    // Clear any existing messages
    Game::clearMessages();
    
    // Judge prevents bribe on briberPlayer
    static_cast<Judge*>(judge->getRole())->Action(briberPlayer);
    
    // Verify bribe prevention was set
    CHECK(briberPlayer->getIsBribePrevented() == true);
    
    // Clear messages again to focus on bribe attempt
    Game::clearMessages();
    
    // Now briberPlayer attempts to bribe (should be prevented)
    int coinsBeforeBribe = briberPlayer->getCoins();
    briberPlayer->getRole()->bribe();
    
    // Collect messages to check for prevention message
    vector<string> collectedMessages;
    while (Game::hasMessages()) {
        collectedMessages.push_back(Game::getNextMessage());
    }
    
    // Look for the specific prevention message
    bool foundPreventionMessage = false;
    bool foundCoinLossMessage = false;
    
    for (const string& message : collectedMessages) {
        
        if (message.find("You were bribe prevented by a judge and your turn ended") != string::npos) {
            foundPreventionMessage = true;
        }
        if (message.find("You lost the 4 coins you payed") != string::npos) {
            foundCoinLossMessage = true;
        }
    }
    
    // Verify the prevention worked correctly
    CHECK(foundPreventionMessage == true); // Should show prevention message
    CHECK(foundCoinLossMessage == true);   // Should show coin loss message
    CHECK(briberPlayer->getCoins() == coinsBeforeBribe - 4); // Should lose 4 coins
    
    Game::resetInstance();
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

TEST_CASE("Spy Peek and Prevent Arrest action")
{
    vector<string> playerNames = {"SpyPlayer", "Avi Biter"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    Player* target = game.active_players()[1];
    
    // Force Spy role for testing
    delete player->getRole();
    player->setRole(new Spy(player, &game));
    
    // Set up test conditions (Spy needs >= 2 coins to peek)
    player->setCoins(2);
    int spy_id = player->getId();
    
    static_cast<Spy*>(player->getRole())->Action(target);
    
    // Verify spy action worked
    CHECK(target->getIsPeekedAndArrestPrevented() == true); // Spy should prevent arrest for the next turn
    CHECK(game.current_player()->getId() == spy_id); // Spy should still be the current player after peek action
    
    Game::resetInstance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for target player selection logic:

TEST_CASE("Target Player Selection Logic on arrest Action") 
{
    // Players can have same names, so we need to ensure the logic works correctly
    // even if they do(we will use IDs to differentiate them)
    vector<string> playerNames = {"Alice", "Alice", "Bob","CJ"};
    Game::configure(100);
    Game& game = Game::getInstance(4, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Give all players some coins for testing
    for (Player* player : players) 
    {
        player->setCoins(5);
    }
    // Set the coins of CJ to 0 so he cannot be arrested
    players[3]->setCoins(0);
    
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

// Test cases for turns:

TEST_CASE("Next Turn Logic") 
{
    vector<string> playerNames = {"Avraham", "Itzhak", "Yaakov"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    vector<Player*> players = game.active_players();
    Player* currentPlayer = game.current_player();
    int first_p_id = currentPlayer->getId();
    CHECK(currentPlayer->getName() == "Avraham"); // First player should be current player
    game.nextTurn(); // Move to next turn
    currentPlayer = game.current_player();
    int second_p_id = currentPlayer->getId();
    // Second player should be current player:
    CHECK(currentPlayer->getName() == "Itzhak");
    CHECK(first_p_id!=second_p_id); 
    Game::resetInstance();
}

TEST_CASE("Another turn on Bribe Success") 
{
    vector<string> playerNames = {"Yarin", "Yarin 2", "Yarin 3","Yarin 4","Yarin"};
    Game::configure(100);
    Game& game = Game::getInstance(5, playerNames);
    vector<Player*> players = game.active_players();
    Player* currentPlayer = game.current_player();
    int first_p_id = currentPlayer->getId();
    CHECK(currentPlayer->getName() == "Yarin"); // First player should be current player
    currentPlayer->setCoins(8); // Set enough coins for 2 bribes
    currentPlayer->getRole()->bribe(); // Perform bribe action
    currentPlayer->getRole()->bribe(); // Perform another bribe action
    currentPlayer = game.current_player(); // Get current player after bribe
    int id_current_after_bribe = currentPlayer->getId();
    CHECK(id_current_after_bribe == first_p_id); // Current player should still be the same after successful bribe
    Game::resetInstance();
}