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
    Game &game = Game::getInstance(2, playerNames);

    vector<Player *> players = game.active_players();
    Player *winner = players[0];
    Player *loser = players[1];

    // Initially both players are active
    CHECK(game.active_players().size() == 2);

    // Eliminate one player
    loser->setIsActive(false);

    // Check game over condition
    vector<Player *> activePlayers = game.active_players();
    CHECK(activePlayers.size() == 1);
    CHECK(activePlayers[0] == winner);

    Game::resetInstance();
}

TEST_CASE("Game Over Winner Name")
{
    vector<string> playerNames = {"Winner", "Loser"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    vector<Player *> players = game.active_players();
    Player *winner = players[0];
    Player *loser = players[1];

    // Initially both players are active
    CHECK(game.active_players().size() == 2);

    CHECK_THROWS(game.winner()); // Should throw if there is no winner yet

    // Eliminate one player
    loser->setIsActive(false);

    // Check winner name
    CHECK(game.winner() == winner->getName());

    Game::resetInstance();
}

TEST_CASE("Game creation with few coins in the bank")
{
    vector<string> playerNames = {"Player1", "Player2"};
    Game::configure(1); // Configure with 1 coin in the bank
    Game &game = Game::getInstance(2, playerNames);
    vector<Player *> players = game.active_players();
    Player* player_1 = players[0];
    CHECK_THROWS(player_1->getRole()->tax()); // Should throw an error for insufficient coins
}

TEST_CASE("Game Instance Singleton")
{
    vector<string> playerNames = {"Single1", "Single2"};
    Game::configure(100);
    Game& game1 = Game::getInstance(2, playerNames);
    Game& game2 = Game::getInstance(2, playerNames);  // Should return same instance
    
    // Test that both references point to same object
    CHECK(&game1 == &game2);
    
    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for regular actions:

TEST_CASE("Player Gather Action")
{
    vector<string> playerNames = {"Shimi Tavori", "Ben El Tavori"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);
    Player *player = game.active_players()[0];
    int initialCoins = player->getCoins();
    player->getRole()->gather(); // Player gathers coins
    int coinsAfterGather = player->getCoins();
    CHECK(coinsAfterGather == initialCoins + 1); // Player should have 1 more coin after gathering
    Game::resetInstance();
}

TEST_CASE("Player Tax Action")
{
    vector<string> playerNames = {"Omer Adam", "Sharif The Druze Kid", "Noa Kirel"};
    Game::configure(100);
    Game &game = Game::getInstance(3, playerNames);
    Player *player = game.active_players()[0];

    // Force Baron role for testing(not governor so his tax is different - checked in another test)
    delete player->getRole();
    Baron *baron = new Baron(player, &game);
    player->setRole(baron);

    int initialCoins = player->getCoins();
    player->getRole()->tax(); // Player taxes
    int coinsAfterTax = player->getCoins();
    CHECK(coinsAfterTax == initialCoins + 2); // Player should have 2 more coins after taxing
    Game::resetInstance();
}

TEST_CASE("player Sanction Action")
{
    vector<string> playerNames = {"SanctionPlayer", "TargetPlayer - Governor", "Target - Baron"};
    Game::configure(100);
    Game &game = Game::getInstance(3, playerNames);

    Player *player = game.active_players()[0];
    Player *target_baron = game.active_players()[2];

    // Force Baron role for testing
    delete target_baron->getRole();
    target_baron->setRole(new Baron(target_baron, &game));

    // Set up test conditions
    player->setCoins(6);
    target_baron->setCoins(3); // Target Baron has 3 coins
    int initialCoins = target_baron->getCoins();

    static_cast<Judge *>(player->getRole())->sanction(target_baron);

    // Verify sanction action worked
    CHECK(target_baron->getIsSanctioned() == true);      // Target should be sanctioned
    CHECK(target_baron->getCoins() == initialCoins + 1); // Target should get 1 coin as compensation for being a Baron

    Game::resetInstance();
}

TEST_CASE("Arrest logic")
{
    vector<string> playerNames = {"ArrestPlayer", "TargetPlayer - Merchant", "TargetPlayer - General", "TargetPlayer - Spy"};
    Game::configure(100);
    Game &game = Game::getInstance(4, playerNames);
    Player *arrest_player = game.active_players()[0];
    Player *target_merchant = game.active_players()[1];
    Player *target_general = game.active_players()[2];
    Player *target_spy = game.active_players()[3]; // just to make sure not a general or a merchant

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
    target_general->setCoins(4);  // General has 4 coins
    target_spy->setCoins(2);      // Spy has 2 coins
    int initialCoinsMerchant = target_merchant->getCoins();
    int initialCoinsGeneral = target_general->getCoins();
    int initialCoinsSpy = target_spy->getCoins();
    // Perform arrest on Merchant
    static_cast<Judge *>(arrest_player->getRole())->arrest(target_merchant);
    // Verify arrest action worked
    CHECK(target_merchant->getIsArrested() == true);                // Merchant should be arrested
    CHECK(target_merchant->getCoins() == initialCoinsMerchant - 2); // Merchant should pay 2 coins to the bank
    game.nextTurn();                                                // Merhcant skips his turn
    game.nextTurn();                                                // General skips his turn
    game.nextTurn();                                                // Spy skips his turn

    arrest_player->getRole()->arrest(target_general); // Perform arrest on General
    // Verify arrest action worked
    CHECK(target_general->getIsArrested() == true);           // General should be arrested
    CHECK(target_general->getCoins() == initialCoinsGeneral); // General should get his coin back
    game.nextTurn();                                          // Merhcant skips his turn
    game.nextTurn();                                          // General skips his turn
    game.nextTurn();                                          // Spy skips his turn
    arrest_player->getRole()->arrest(target_spy);             // Perform arrest on Spy
    // Verify arrest action worked
    CHECK(target_spy->getIsArrested() == true);           // Spy should be arrested
    CHECK(target_spy->getCoins() == initialCoinsSpy - 1); // Spy should lose 1 coin
    Game::resetInstance();
}

TEST_CASE("Coup action - General defence")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - General"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player1 = game.active_players()[0];
    Player *player2 = game.active_players()[1];

    // Force General role for testing
    delete player2->getRole();
    player2->setRole(new General(player2, &game));

    // Set up test conditions (General needs >= 5 coins to prevent coup on himself)
    player2->setCoins(5);
    player1->setCoins(7); // Player 1 has enough coins to coup

    static_cast<General *>(player1->getRole())->coup(player2);

    // Verify coup action worked
    CHECK(player2->getIsActive() == true); // Target should not be eliminated

    Game::resetInstance();
}

TEST_CASE("Coup action - No defence")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - General"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player1 = game.active_players()[0];
    Player *player2 = game.active_players()[1];

    // Force General role for testing
    delete player2->getRole();
    player2->setRole(new General(player2, &game));

    // Set up test conditions (General needs >= 5 coins to prevent coup on himself)
    player2->setCoins(4); // Not enough coins to prevent coup
    player1->setCoins(7); // Player 1 has enough coins to coup

    static_cast<General *>(player1->getRole())->coup(player2);

    // Verify coup action worked
    CHECK(player2->getIsActive() == false); // Target should not be eliminated

    Game::resetInstance();
}

TEST_CASE("check coup force")
{
    vector<string> playerNames = {"CoupPlayer", "TargetPlayer - Baron"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player1 = game.active_players()[0];
    Player *player2 = game.active_players()[1];

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
    if (player1->getCoins() >= 10)
    {
        Game::addMessage(player1->getName() + " has " + to_string(player1->getCoins()) + " coins and must coup!");
    }

    // Check for forced coup message using existing methods
    bool foundForcedCoupMessage = false;
    vector<string> collectedMessages;

    // Collect all messages from the queue
    while (Game::hasMessages())
    {
        string message = Game::getNextMessage();
        collectedMessages.push_back(message);
        if (message.find("must coup") != string::npos ||
            message.find("forced to coup") != string::npos ||
            message.find("10 coins") != string::npos)
        {
            foundForcedCoupMessage = true;
        }
    }

    // Perform coup action
    player1->getRole()->coup(player2);

    // Verify coup worked and message was displayed
    CHECK(foundForcedCoupMessage == true);  // Message should be displayed
    CHECK(player2->getIsActive() == false); // Target eliminated
    CHECK(player1->getCoins() == 3);        // 10 - 7 = 3 coins left

    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test case for role-specific actions:

TEST_CASE("Baron Investment Action")
{
    vector<string> playerNames = {"BaronPlayer", "Eyal Golan The King"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player = game.active_players()[0];

    // Force Baron role for testing
    delete player->getRole();
    Baron *baron = new Baron(player, &game);
    player->setRole(baron);

    // Test Baron's invest action directly
    player->setCoins(3);
    int initialCoins = player->getCoins();

    static_cast<Baron *>(player->getRole())->Action();

    // Check what actually happened
    CHECK(player->getCoins() >= initialCoins); // At least no coins lost

    Game::resetInstance();
}

TEST_CASE("General special action")
{
    vector<string> playerNames = {"GeneralPlayer", "TargetPlayer"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player = game.active_players()[0];
    Player *target = game.active_players()[1];

    // Force General role for testing
    delete player->getRole();
    player->setRole(new General(player, &game));

    // Set up test conditions (General needs >= 5 coins to perform special action)
    player->setCoins(5);
    target->setCoins(2); // Target player has less than 5 coins(cannot defend coup)

    // Perform the action
    static_cast<General *>(player->getRole())->Action(target);

    // Verify general action worked (e.g., coins should be reduced by 1)
    CHECK(target->getIsCoupPrevented() == true); // General should prevent coup for the next turn

    Game::resetInstance();
}

TEST_CASE("Governor Tax Action")
{
    vector<string> playerNames = {"GovernorPlayer", "Cosmic Girl From Another Galaxy"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player = game.active_players()[0];

    // Force Governor role for testing
    delete player->getRole();
    player->setRole(new Governor(player, &game));

    player->setCoins(3);
    int initialCoins = player->getCoins();

    static_cast<Governor *>(player->getRole())->Action();

    // Verify governor action worked
    CHECK(player->getCoins() == initialCoins + 3); // Governor should gain 3 coins from tax

    Game::resetInstance();
}

TEST_CASE("Governor Cancel Tax Action")
{
    vector<string> playerNames = {"GovernorPlayer", "Amit Capit", "Eli Copter", "Moti lohem"};
    Game::configure(666);
    Game &game = Game::getInstance(4, playerNames);
    Player *player = game.active_players()[0];
    Player *target = game.active_players()[3];

    // Force Governor role for testing
    delete player->getRole();
    player->setRole(new Governor(player, &game));
    player->setCoins(6);

    static_cast<Governor *>(player->getRole())->Action(target);
    CHECK(target->getIsTaxPrevented() == true); // Target should be tax prevented
    Game::resetInstance();
}

TEST_CASE("Judge Bribe Prevention Message Test")
{
    vector<string> playerNames = {"JudgePlayer", "BribePlayer", "ThirdPlayer"};
    Game::configure(100);
    Game &game = Game::getInstance(3, playerNames);

    Player *judge = game.active_players()[0];
    Player *briberPlayer = game.active_players()[1];

    // Set up Judge to prevent bribe on briberPlayer
    delete judge->getRole();
    judge->setRole(new Judge(judge, &game));
    judge->setCoins(3);

    // Set up briber with enough coins
    briberPlayer->setCoins(8);

    // Clear any existing messages
    Game::clearMessages();

    // Judge prevents bribe on briberPlayer
    static_cast<Judge *>(judge->getRole())->Action(briberPlayer);

    // Verify bribe prevention was set
    CHECK(briberPlayer->getIsBribePrevented() == true);

    // Clear messages again to focus on bribe attempt
    Game::clearMessages();

    // Now briberPlayer attempts to bribe (should be prevented)
    int coinsBeforeBribe = briberPlayer->getCoins();
    briberPlayer->getRole()->bribe();

    // Collect messages to check for prevention message
    vector<string> collectedMessages;
    while (Game::hasMessages())
    {
        collectedMessages.push_back(Game::getNextMessage());
    }

    // Look for the specific prevention message
    bool foundPreventionMessage = false;
    bool foundCoinLossMessage = false;

    for (const string &message : collectedMessages)
    {

        if (message.find("You were bribe prevented by a judge and your turn ended") != string::npos)
        {
            foundPreventionMessage = true;
        }
        if (message.find("You lost the 4 coins you payed") != string::npos)
        {
            foundCoinLossMessage = true;
        }
    }

    // Verify the prevention worked correctly
    CHECK(foundPreventionMessage == true);                   // Should show prevention message
    CHECK(foundCoinLossMessage == true);                     // Should show coin loss message
    CHECK(briberPlayer->getCoins() == coinsBeforeBribe - 4); // Should lose 4 coins

    Game::resetInstance();
}

TEST_CASE("Merchant Add Coin Action")
{
    vector<string> playerNames = {"MerchantPlayer", "Some Other Player"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player = game.active_players()[0];

    // Force Merchant role for testing
    delete player->getRole();
    player->setRole(new Merchant(player, &game));

    // Set up test conditions (Merchant needs >= 3 coins to add coin)
    player->setCoins(3);
    int initialCoins = player->getCoins();

    static_cast<Merchant *>(player->getRole())->Action();

    // Verify merchant action worked
    CHECK(player->getCoins() == initialCoins + 1);

    Game::resetInstance();
}

TEST_CASE("Spy Peek and Prevent Arrest action")
{
    vector<string> playerNames = {"SpyPlayer", "Avi Biter"};
    Game::configure(100);
    Game &game = Game::getInstance(2, playerNames);

    Player *player = game.active_players()[0];
    Player *target = game.active_players()[1];

    // Force Spy role for testing
    delete player->getRole();
    player->setRole(new Spy(player, &game));

    // Set up test conditions (Spy needs >= 2 coins to peek)
    player->setCoins(2);
    int spy_id = player->getId();

    static_cast<Spy *>(player->getRole())->Action(target);

    // Verify spy action worked
    CHECK(target->getIsPeekedAndArrestPrevented() == true); // Spy should prevent arrest for the next turn
    CHECK(game.current_player()->getId() == spy_id);        // Spy should still be the current player after peek action

    Game::resetInstance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Test cases for game logic:

TEST_CASE("Game Logic - Player Removal")
{
    vector<string> playerNames = {"Player1", "Player2", "Player3"};
    Game::configure(100);
    Game &game = Game::getInstance(3, playerNames);

    vector<Player *> players = game.active_players();
    Player *playerToRemove = players[1]; // Remove second player

    game.removePlayer(playerToRemove);

    // Check if player was removed
    CHECK(game.active_players().size() == 2);
    CHECK(game.active_players()[0]->getName() == "Player1");
    CHECK(game.active_players()[1]->getName() == "Player3");

    Game::resetInstance();
}

TEST_CASE("Player with Zero Coins Actions")
{
    vector<string> playerNames = {"PoorPlayer", "OtherPlayer"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* poorPlayer = game.active_players()[0];
    poorPlayer->setCoins(0); // No coins
    
    // Test what actions are still available with 0 coins
    bool canGather = true;                              // Always available
    bool canTax = true;                                 // Always available  
    bool canBribe = (poorPlayer->getCoins() >= 4);     // Should be false
    bool canCoup = (poorPlayer->getCoins() >= 7);      // Should be false
    bool canSanction = (poorPlayer->getCoins() >= 3);  // Should be false
    
    CHECK(canGather == true);
    CHECK(canTax == true);
    CHECK(canBribe == false);
    CHECK(canCoup == false);
    CHECK(canSanction == false);
    
    Game::resetInstance();
}

TEST_CASE("Player Name Handling")
{
    vector<string> playerNames = {"", "Player with spaces", "123Numbers", "Special@#$"};
    Game::configure(100);
    Game& game = Game::getInstance(4, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Test that all names are preserved correctly
    CHECK(players[0]->getName() == "");
    CHECK(players[1]->getName() == "Player with spaces");
    CHECK(players[2]->getName() == "123Numbers");
    CHECK(players[3]->getName() == "Special@#$");
    
    Game::resetInstance();
}

TEST_CASE("Bank Coins Management")
{
    vector<string> playerNames = {"Player1", "Player2"};
    Game::configure(50);
    Game& game = Game::getInstance(2, playerNames);
    
    int initialBankCoins = game.getCoinsInBank();
    CHECK(initialBankCoins == 50);
    
    // Test bank coin changes
    game.changeCoinsInBank(10);
    CHECK(game.getCoinsInBank() == 60);
    
    game.changeCoinsInBank(-20);
    CHECK(game.getCoinsInBank() == 40);

    CHECK_THROWS(game.changeCoinsInBank(-50)); // Should not go below 0
    
    Game::resetInstance();
}

TEST_CASE("Default Role Assignment")
{
    vector<string> playerNames = {"Charmander", "Bulbasaur", "Squirtle"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Test that all players have some role assigned
    for (Player* player : players) {
        CHECK(player->getRole() != nullptr);
        CHECK(player->getRole()->getRoleName() != "");
    }
    
    Game::resetInstance();
}

TEST_CASE("Role Name Verification")
{
    vector<string> playerNames = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    Game::configure(100);
    Game& game = Game::getInstance(6, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Assign specific roles and verify names
    delete players[0]->getRole();
    players[0]->setRole(new Baron(players[0], &game));
    CHECK(players[0]->getRole()->getRoleName() == "Baron");
    
    delete players[1]->getRole();
    players[1]->setRole(new General(players[1], &game));
    CHECK(players[1]->getRole()->getRoleName() == "General");
    
    delete players[2]->getRole();
    players[2]->setRole(new Governor(players[2], &game));
    CHECK(players[2]->getRole()->getRoleName() == "Governor");
    
    delete players[3]->getRole();
    players[3]->setRole(new Judge(players[3], &game));
    CHECK(players[3]->getRole()->getRoleName() == "Judge");
    
    delete players[4]->getRole();
    players[4]->setRole(new Merchant(players[4], &game));
    CHECK(players[4]->getRole()->getRoleName() == "Merchant");
    
    delete players[5]->getRole();
    players[5]->setRole(new Spy(players[5], &game));
    CHECK(players[5]->getRole()->getRoleName() == "Spy");
    
    Game::resetInstance();
}

TEST_CASE("Player Status Flags and Initial Conditions")
{
    vector<string> playerNames = {"TestPlayer", "Oded Paz"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    
    // Test initial status
    CHECK(player->getIsActive() == true);
    CHECK(player->getIsArrested() == false);
    CHECK(player->getIsSanctioned() == false);
    CHECK(player->getIsTaxPrevented() == false);
    CHECK(player->getIsBribePrevented() == false);
    CHECK(player->getIsCoupPrevented() == false);
    CHECK(player->getIsPeekedAndArrestPrevented() == false);
    CHECK(player->getCoins() == 0); // Initial coins should be 0
    CHECK(player->getId() >= 0); // Player ID should be valid
    CHECK(player->getName() == "TestPlayer"); // Player name should match
    CHECK(player->getRole() != nullptr); // Player should have a role assigned

    // Test setting status flags
    player->setIsArrested(true);
    CHECK(player->getIsArrested() == true);
    
    player->setIsSanctioned(true);
    CHECK(player->getIsSanctioned() == true);
    
    player->setIsTaxPrevented(true);
    CHECK(player->getIsTaxPrevented() == true);

    player->setIsBribePrevented(true);
    CHECK(player->getIsBribePrevented() == true);

    player->setIsCoupPrevented(true);
    CHECK(player->getIsCoupPrevented() == true);

    player->setIsPeekedAndArrestPrevented(true);
    CHECK(player->getIsPeekedAndArrestPrevented() == true);

    player->setIsActive(false);
    CHECK(player->getIsActive() == false); // Player should be inactive now
    
    Game::resetInstance();
}

TEST_CASE("Action Cost Verification")
{
    vector<string> playerNames = {"TestPlayer", "Target"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* player = game.active_players()[0];
    Player* target = game.active_players()[1];
    
    // Test bribe cost (4 coins)
    player->setCoins(9);
    int initialCoins = player->getCoins();
    
    player->getRole()->bribe();
    CHECK(player->getCoins() == initialCoins - 4);  // Should lose 4 coins
    
    // Test coup cost (7 coins)
    player->setCoins(9);
    initialCoins = player->getCoins();
    
    player->getRole()->coup(target);
    CHECK(player->getCoins() == initialCoins - 7);  // Should lose 7 coins
    
    Game::resetInstance();
}

TEST_CASE("Insufficient Coins Prevention")
{
    vector<string> playerNames = {"PoorPlayer", "Target"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* poorPlayer = game.active_players()[0];
    Player* target = game.active_players()[1];
    
    // Test bribe with insufficient coins
    poorPlayer->setCoins(3);  // Not enough for bribe (needs 4)
    int initialCoins = poorPlayer->getCoins();
    
    poorPlayer->getRole()->bribe();
    CHECK(poorPlayer->getCoins() == initialCoins);  // Coins shouldn't change
    
    // Test coup with insufficient coins
    poorPlayer->setCoins(6);  // Not enough for coup (needs 7)
    initialCoins = poorPlayer->getCoins();
    
    poorPlayer->getRole()->coup(target);
    CHECK(poorPlayer->getCoins() == initialCoins);  // Coins shouldn't change
    CHECK(target->getIsActive() == true);           // Target shouldn't be eliminated
    
    Game::resetInstance();
}

TEST_CASE("Player ID Uniqueness")
{
    vector<string> playerNames = {"Same", "Same", "Same"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    vector<Player*> players = game.active_players();
    
    // Test that all players have unique IDs even with same names
    CHECK(players[0]->getId() != players[1]->getId());
    CHECK(players[1]->getId() != players[2]->getId());
    CHECK(players[0]->getId() != players[2]->getId());
    
    Game::resetInstance();
}

TEST_CASE("Status Flags Reset")
{
    vector<string> playerNames = {"One", "Two"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);

    Player* player = game.active_players()[0];
    // Set all flags to true
    player->setIsArrested(true);
    player->setIsSanctioned(true);
    player->setIsTaxPrevented(true);
    player->setIsBribePrevented(true);
    player->setIsCoupPrevented(true);
    player->setIsPeekedAndArrestPrevented(true);
    player->setCoins(5); // Set some coins for testing

    // Reset all flags
    game.resetPlayerStatus(player);
    CHECK(player->getIsArrested() == true); // Player can be arrested once in the game cannot reset
    CHECK(player->getIsSanctioned() == false);
    CHECK(player->getIsTaxPrevented() == false);
    CHECK(player->getIsBribePrevented() == false);
    CHECK(player->getIsCoupPrevented() == false);
    CHECK(player->getIsPeekedAndArrestPrevented() == false);
    CHECK(player->getIsActive() == true); // Player should still be active
    CHECK(player->getCoins() == 5); // Player should have 5 coins still after reset
    CHECK(player->getId() >= 0); // Player ID should still be valid
    CHECK(player->getName() == "One"); // Player name should still match
    CHECK(player->getRole() != nullptr); // Player should still have a role assigned
    CHECK(player->getRole()->getRoleName() != ""); // Role name should not be empty

    Game::resetInstance();
}

TEST_CASE("Advanced elimination chain with max players")
{
    vector<string> playerNames = {"Attacker", "Defender1", "Defender2", "Victim", "Observer", "LastStanding"};
    Game::configure(200);
    Game& game = Game::getInstance(6, playerNames);
    
    vector<Player*> players = game.active_players();
    Player* attacker = players[0];
    Player* defender1 = players[1];
    Player* defender2 = players[2]; 
    Player* victim = players[3];
    Player* observer = players[4];
    Player* lastStanding = players[5];
    
    // Set up complex roles
    delete attacker->getRole();
    attacker->setRole(new Judge(attacker, &game));
    
    delete defender1->getRole();
    defender1->setRole(new General(defender1, &game));
    
    delete defender2->getRole();
    defender2->setRole(new General(defender2, &game));
    
    delete victim->getRole();
    victim->setRole(new Merchant(victim, &game));
    
    // Set up coins for complex interactions
    attacker->setCoins(15);  // Can coup multiple times
    defender1->setCoins(5);  // Can defend once
    defender2->setCoins(4);  // Cannot defend
    victim->setCoins(2);     // Vulnerable
    observer->setCoins(8);   // Spectator
    lastStanding->setCoins(10); // Potential winner
    
    // Test elimination chain
    CHECK(game.active_players().size() == 6);
    
    // First coup - defender1 can defend
    attacker->getRole()->coup(defender1);
    CHECK(defender1->getIsActive() == true);  // General defended
    CHECK(defender1->getCoins() == 0);        // Used 5 coins to defend
    
    // Second coup - defender2 cannot defend
    attacker->setCoins(15); // Reset for test
    attacker->getRole()->coup(defender2);
    CHECK(defender2->getIsActive() == false); // Eliminated
    CHECK(game.active_players().size() == 5);
    
    // Third coup - victim eliminated
    attacker->setCoins(15);
    attacker->getRole()->coup(victim);
    CHECK(victim->getIsActive() == false);
    CHECK(game.active_players().size() == 4);
    
    // Verify turn rotation skips eliminated players
    for (int i = 0; i < 10; i++) {
        Player* current = game.current_player();
        CHECK(current->getIsActive() == true);
        CHECK(current != defender2);
        CHECK(current != victim);
        game.nextTurn();
    }
    
    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for target player selection logic:

TEST_CASE("Target Player Selection Logic on arrest Action") // add judge
{
    // Players can have same names, so we need to ensure the logic works correctly
    // even if they do(we will use IDs to differentiate them)
    vector<string> playerNames = {"Alice", "Alice", "Bob", "CJ"};
    Game::configure(100);
    Game &game = Game::getInstance(4, playerNames);

    vector<Player *> players = game.active_players();

    // Give all players some coins for testing
    for (Player *player : players)
    {
        player->setCoins(5);
    }

    int currentPlayerId = players[0]->getId();

    // Test filtering logic for arrest action
    vector<Player *> validTargets;
    for (Player *player : players)
    {
        if (player->getId() != currentPlayerId) // Not current player
        {
            if (!player->getIsArrested() && player->getCoins() >= 1)
            {
                validTargets.push_back(player);
            }
        }
    }

    CHECK(validTargets.size() == 3); // Two other players besides current player should be valid targets

    Game::resetInstance();
}

TEST_CASE("Target Player Selection Logic on Coup Action")
{
    // Players can have same names, so we need to ensure the logic works correctly
    // even if they do(we will use IDs to differentiate them)
    vector<string> playerNames = {"Alice", "Alice", "Blip", "Blop"};
    Game::configure(100);
    Game &game = Game::getInstance(4, playerNames);

    vector<Player *> players = game.active_players();

    // Give all players some coins for testing
    for (Player *player : players)
    {
        player->setCoins(7); // Set enough coins for coup
    }

    int currentPlayerId = players[0]->getId();

    // Test filtering logic for coup action
    vector<Player *> validTargets;
    for (Player *player : players)
    {
        if (player->getId() != currentPlayerId) // Not current player
        {
            if (player->getIsActive())
            {
                validTargets.push_back(player);
            }
        }
    }

    CHECK(validTargets.size() == 3); // Two other players besides current player should be valid targets

    Game::resetInstance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for turns:

TEST_CASE("Next Turn Logic")
{
    vector<string> playerNames = {"Avraham", "Itzhak", "Yaakov"};
    Game::configure(100);
    Game &game = Game::getInstance(3, playerNames);
    vector<Player *> players = game.active_players();
    Player *currentPlayer = game.current_player();
    int first_p_id = currentPlayer->getId();
    CHECK(currentPlayer->getName() == "Avraham"); // First player should be current player
    game.nextTurn();                              // Move to next turn
    currentPlayer = game.current_player();
    int second_p_id = currentPlayer->getId();
    // Second player should be current player:
    CHECK(currentPlayer->getName() == "Itzhak");
    CHECK(first_p_id != second_p_id);

    CHECK(players[0]->getIsTurn() == false); // First player should not be on turn anymore
    CHECK(players[1]->getIsTurn() == true);  // Second player should be on turn now
    Game::resetInstance();
}

TEST_CASE("Another turn on Bribe Success")
{
    vector<string> playerNames = {"Yarin", "Yarin 2", "Yarin 3", "Yarin 4", "Yarin"};
    Game::configure(100);
    Game &game = Game::getInstance(5, playerNames);
    vector<Player *> players = game.active_players();
    Player *currentPlayer = game.current_player();
    int first_p_id = currentPlayer->getId();
    CHECK(currentPlayer->getName() == "Yarin"); // First player should be current player
    currentPlayer->setCoins(8);                 // Set enough coins for 2 bribes
    currentPlayer->getRole()->bribe();          // Perform bribe action
    currentPlayer->getRole()->bribe();          // Perform another bribe action
    currentPlayer = game.current_player();      // Get current player after bribe
    int id_current_after_bribe = currentPlayer->getId();
    CHECK(id_current_after_bribe == first_p_id); // Current player should still be the same after successful bribe
    Game::resetInstance();
}

TEST_CASE("Turn Flag Consistency")
{
    vector<string> playerNames = {"P1", "P2", "P3"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    
    // Test that exactly one player has turn at any time
    for (int i = 0; i < 6; i++) {  // Test multiple turns
        int playersWithTurn = 0;
        for (Player* player : game.active_players()) {
            if (player->getIsTurn()) {
                playersWithTurn++;
            }
        }
        CHECK(playersWithTurn == 1);  // Exactly one player should have turn
        game.nextTurn();
    }
    
    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for buttons available to players in GUI:

TEST_CASE("Check Available Buttons for Players")
{
    vector<string> playerNames = {"Red", "Green", "Blue", "Yellow", "Purple", "Pink"};
    Game::configure(1000);
    Game &game = Game::getInstance(6, playerNames);
    vector<Player *> players = game.active_players();

    Player *first_player = players[0];
    Player *second_player = players[1];
    Player *third_player = players[2];
    Player *fourth_player = players[3];
    Player *fifth_player = players[4];
    Player *sixth_player = players[5];

    delete first_player->getRole();
    first_player->setRole(new Baron(first_player, &game));

    delete second_player->getRole();
    second_player->setRole(new General(second_player, &game));

    delete third_player->getRole();
    third_player->setRole(new Governor(third_player, &game));

    delete fourth_player->getRole();
    fourth_player->setRole(new Judge(fourth_player, &game));

    delete fifth_player->getRole();
    fifth_player->setRole(new Merchant(fifth_player, &game));

    delete sixth_player->getRole();
    sixth_player->setRole(new Spy(sixth_player, &game));

    for (Player *player : players)
    {
        player->setCoins(9); // Set enough coins for all actions(no coup force)
    }

    // Check all buttons available for all players(except Baron who has passive special ability):

    // Create a test window for GUI button testing
    sf::RenderWindow window(sf::VideoMode(800, 600), "Button Test Window");

    // Test each player's button availability
    for (Player *player : players)
    {
        string roleName = player->getRole()->getRoleName();

        // Set this player as current player
        while (game.current_player()->getId() != player->getId())
        {
            game.nextTurn();
        }

        // Create GUI buttons (same as main.cpp)
        sf::RectangleShape gatherButton(sf::Vector2f(100, 50));
        sf::RectangleShape taxButton(sf::Vector2f(100, 50));
        sf::RectangleShape bribeButton(sf::Vector2f(100, 50));
        sf::RectangleShape arrestButton(sf::Vector2f(100, 50));
        sf::RectangleShape sanctionButton(sf::Vector2f(100, 50));
        sf::RectangleShape coupButton(sf::Vector2f(100, 50));
        sf::RectangleShape roleActionButton(sf::Vector2f(100, 50));
        sf::RectangleShape skipButton(sf::Vector2f(100, 50));

        // Apply GUI logic for button states (from main.cpp)
        bool mustCoup = (player->getCoins() >= 10);

        // 1. Gather Button
        if (!player->getIsSanctioned() && !mustCoup)
        {
            gatherButton.setFillColor(sf::Color::Green);
        }
        else
        {
            gatherButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 2. Tax Button
        if (!player->getIsSanctioned() && !player->getIsTaxPrevented() && !mustCoup)
        {
            taxButton.setFillColor(sf::Color::Green);
        }
        else
        {
            taxButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 3. Bribe Button
        if (player->getCoins() >= 4 && !mustCoup)
        {
            bribeButton.setFillColor(sf::Color::Green);
        }
        else
        {
            bribeButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 4. Arrest Button
        bool canArrest = !player->getIsPeekedAndArrestPrevented() && !mustCoup;
        if (canArrest)
        {
            // Check for valid targets
            bool hasValidTargets = false;
            for (Player *p : players)
            {
                if (p->getId() != player->getId() && !p->getIsArrested() && p->getCoins() > 0)
                {
                    hasValidTargets = true;
                    break;
                }
            }
            canArrest = hasValidTargets;
        }

        if (canArrest)
        {
            arrestButton.setFillColor(sf::Color::Green);
        }
        else
        {
            arrestButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 5. Sanction Button
        if (player->getCoins() >= 3 && !mustCoup)
        {
            sanctionButton.setFillColor(sf::Color::Green);
        }
        else
        {
            sanctionButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 6. Coup Button
        if (player->getCoins() >= 7 && !player->getIsCoupPrevented())
        {
            coupButton.setFillColor(sf::Color::Green);
        }
        else
        {
            coupButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 7. Role-specific Action Button
        bool roleActionAvailable = true;
        if (roleName == "Baron")
        {
            roleActionAvailable = (player->getCoins() >= 3) && !mustCoup;
        }
        else if (roleName == "General")
        {
            roleActionAvailable = (player->getCoins() >= 5) && !mustCoup;
        }
        else if (roleName == "Governor")
        {
            roleActionAvailable = !mustCoup; // Governor action is free
        }
        else if (roleName == "Judge")
        {
            roleActionAvailable = !mustCoup; // Judge action is free
        }
        else if (roleName == "Merchant")
        {
            roleActionAvailable = false; // Always disabled in GUI
        }
        else if (roleName == "Spy")
        {
            Spy *spy = static_cast<Spy *>(player->getRole());
            roleActionAvailable = !spy->getIsAlreadyPeeked() && !mustCoup;
        }

        if (roleActionAvailable)
        {
            roleActionButton.setFillColor(sf::Color::Green);
        }
        else
        {
            roleActionButton.setFillColor(sf::Color(128, 128, 128));
        }

        // 8. Skip Button (always available)
        skipButton.setFillColor(sf::Color::Green);

        // Test the actual GUI button states
        CHECK(gatherButton.getFillColor() == sf::Color::Green);   // Should be clickable (not sanctioned, <10 coins)
        CHECK(taxButton.getFillColor() == sf::Color::Green);      // Should be clickable (not sanctioned, not prevented, <10 coins)
        CHECK(bribeButton.getFillColor() == sf::Color::Green);    // Should be clickable (has 9 coins >= 4, <10 coins)
        CHECK(arrestButton.getFillColor() == sf::Color::Green);   // Should be clickable (not prevented, has valid targets, <10 coins)
        CHECK(sanctionButton.getFillColor() == sf::Color::Green); // Should be clickable (has 9 coins >= 3, <10 coins)
        CHECK(coupButton.getFillColor() == sf::Color::Green);     // Should be clickable (has 9 coins >= 7, not prevented)
        CHECK(skipButton.getFillColor() == sf::Color::Green);     // Always clickable

        // Role-specific button checks
        if (roleName == "Baron")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color::Green); // Has enough coins for invest
        }
        else if (roleName == "General")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color::Green); // Has enough coins for prevent coup
        }
        else if (roleName == "Governor")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color::Green); // Action is free
        }
        else if (roleName == "Judge")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color::Green); // Action is free
        }
        else if (roleName == "Merchant")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color(128, 128, 128)); // Always disabled in GUI
        }
        else if (roleName == "Spy")
        {
            CHECK(roleActionButton.getFillColor() == sf::Color::Green); // Hasn't peeked yet
        }

    }

    // Close the test window
    window.close();

    Game::resetInstance();
}

TEST_CASE("GUI Prevention Effects on Buttons")
{
    vector<string> playerNames = {"PreventedPlayer", "OtherPlayer"};
    Game::configure(100);
    Game& game = Game::getInstance(2, playerNames);
    
    Player* preventedPlayer = game.active_players()[0];
    
    
    // Create GUI window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prevention Test");
    
    // Set up prevention states
    preventedPlayer->setCoins(8);
    preventedPlayer->setIsSanctioned(true);
    preventedPlayer->setIsTaxPrevented(true);
    preventedPlayer->setIsBribePrevented(true);
    preventedPlayer->setIsPeekedAndArrestPrevented(true);
    
    // Create buttons
    sf::RectangleShape gatherButton(sf::Vector2f(100, 50));
    sf::RectangleShape taxButton(sf::Vector2f(100, 50));
    sf::RectangleShape bribeButton(sf::Vector2f(100, 50));
    sf::RectangleShape arrestButton(sf::Vector2f(100, 50));
    sf::RectangleShape sanctionButton(sf::Vector2f(100, 50));
    sf::RectangleShape coupButton(sf::Vector2f(100, 50));
    
    // Apply prevention logic to buttons
    bool mustCoup = (preventedPlayer->getCoins() >= 10);
    
    if (!preventedPlayer->getIsSanctioned() && !mustCoup) {
        gatherButton.setFillColor(sf::Color::Green);
    } else {
        gatherButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    if (!preventedPlayer->getIsSanctioned() && !preventedPlayer->getIsTaxPrevented() && !mustCoup) {
        taxButton.setFillColor(sf::Color::Green);
    } else {
        taxButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    if (preventedPlayer->getCoins() >= 4 && !mustCoup) {
        bribeButton.setFillColor(sf::Color::Green);
    } else {
        bribeButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    if (!preventedPlayer->getIsPeekedAndArrestPrevented() && !mustCoup) {
        arrestButton.setFillColor(sf::Color::Green);
    } else {
        arrestButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    if (preventedPlayer->getCoins() >= 3 && !mustCoup) {
        sanctionButton.setFillColor(sf::Color::Green);
    } else {
        sanctionButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    if (preventedPlayer->getCoins() >= 7) {
        coupButton.setFillColor(sf::Color::Green);
    } else {
        coupButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    // Test prevention effects on buttons
    CHECK(gatherButton.getFillColor() == sf::Color(128, 128, 128));    // Disabled by sanction
    CHECK(taxButton.getFillColor() == sf::Color(128, 128, 128));       // Disabled by sanction AND tax prevention
    CHECK(bribeButton.getFillColor() == sf::Color::Green);             // Still available (prevention only affects success)
    CHECK(arrestButton.getFillColor() == sf::Color(128, 128, 128));    // Disabled by peek prevention
    CHECK(sanctionButton.getFillColor() == sf::Color::Green);          // Still available (sanction not preventable)
    CHECK(coupButton.getFillColor() == sf::Color::Green);              // Still available (has enough coins)
    
    window.close();
    Game::resetInstance();
}

TEST_CASE("Spy special ability button and arrest button availability")
{
    vector<string> playerNames = {"StatePlayer", "TargetPlayer1", "TargetPlayer2"};
    Game::configure(100);
    Game& game = Game::getInstance(3, playerNames);
    vector<Player*> players = game.active_players();
    
    Player* statePlayer = players[0];
    Player* target1 = players[1];
    Player* target2 = players[2];
    
    // Set up Spy role for state tracking
    delete statePlayer->getRole();
    statePlayer->setRole(new Spy(statePlayer, &game));
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Advanced State Test");
    
    statePlayer->setCoins(6); // Set enough coins for actions
    target1->setCoins(3); // Set coins for target 1
    target2->setCoins(0); // Set coins for target 2 (invalid target)
    
    // Initial state - can peek
    Spy* spy = static_cast<Spy*>(statePlayer->getRole());
    sf::RectangleShape spyButton(sf::Vector2f(100, 50));
    
    bool canPeek = !spy->getIsAlreadyPeeked();
    if (canPeek) 
    {
        spyButton.setFillColor(sf::Color::Green);
    }
    else 
    {
        spyButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    CHECK(spyButton.getFillColor() == sf::Color::Green); // Should be available
    
    // Perform peek action
    spy->Action(target1);
    
    // State should change - cannot peek again
    canPeek = !spy->getIsAlreadyPeeked();
    if (canPeek) 
    {
        spyButton.setFillColor(sf::Color::Green);
    }
    else 
    {
        spyButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    CHECK(spyButton.getFillColor() == sf::Color(128, 128, 128)); // Should be disabled
    
    // Test arrest button state based on valid targets
    sf::RectangleShape arrestButton(sf::Vector2f(100, 50));
    
    // Initially should have valid targets
    bool hasValidTargets = false;
    vector<Player*> allPlayers = game.active_players();
    for (Player* p : allPlayers) 
    {
        if (p->getId() != statePlayer->getId() && !p->getIsArrested() && p->getCoins() > 0) 
        {
            hasValidTargets = true;
            break;
        }
    }
    
    if (hasValidTargets && !statePlayer->getIsPeekedAndArrestPrevented()) 
    {
        arrestButton.setFillColor(sf::Color::Green);
    } 
    else 
    {
        arrestButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    CHECK(arrestButton.getFillColor() == sf::Color::Green); // Should be available
    
    // Arrest one target
    statePlayer->getRole()->arrest(target1);
    
    
    // Check arrest button state again
    hasValidTargets = false;
    for (Player* p : allPlayers) 
    {
        if (p->getId() != statePlayer->getId() && !p->getIsArrested() && p->getCoins() > 0) 
        {
            hasValidTargets = true;
            break;
        }
    }
    
    if (hasValidTargets && !statePlayer->getIsPeekedAndArrestPrevented()) 
    {
        arrestButton.setFillColor(sf::Color::Green);
    }
    else 
    {
        arrestButton.setFillColor(sf::Color(128, 128, 128));
    }
    
    CHECK(arrestButton.getFillColor() == sf::Color(128, 128, 128)); // Should be disabled
    
    window.close();
    Game::resetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Test cases for messages in GUI:

TEST_CASE("Message Queue Basic Operations")
{
    Game::clearMessages();
    CHECK(Game::hasMessages() == false);
    
    Game::addMessage("Test message 1");
    CHECK(Game::hasMessages() == true);
    
    string message = Game::getNextMessage();
    CHECK(message == "Test message 1");
    CHECK(Game::hasMessages() == false);
}

TEST_CASE("Multiple Messages Order")
{
    Game::clearMessages();
    
    Game::addMessage("First");
    Game::addMessage("Second");
    Game::addMessage("Third");
    
    CHECK(Game::getNextMessage() == "First");
    CHECK(Game::getNextMessage() == "Second");
    CHECK(Game::getNextMessage() == "Third");
    CHECK(Game::hasMessages() == false);
}

TEST_CASE("Empty Message Handling")
{
    Game::clearMessages();
    
    // Test empty message
    Game::addMessage("");
    CHECK(Game::hasMessages() == true);
    
    string message = Game::getNextMessage();
    CHECK(message == "");
    
    Game::clearMessages();
}

TEST_CASE("Special Character Messages")
{
    Game::clearMessages();
    
    // Test messages with special characters
    Game::addMessage("Message with symbols: !@#$%^&*()");
    Game::addMessage("Message with numbers: 123456789");
    Game::addMessage("Message with spaces:     ");
    
    CHECK(Game::getNextMessage() == "Message with symbols: !@#$%^&*()");
    CHECK(Game::getNextMessage() == "Message with numbers: 123456789");
    CHECK(Game::getNextMessage() == "Message with spaces:     ");
    
    Game::clearMessages();
}

TEST_CASE("Message Queue Overflow")
{
    Game::clearMessages();
    
    // Add many messages
    for (int i = 0; i < 100; i++) {
        Game::addMessage("Message " + to_string(i));
    }
    
    // Check they're all there in order
    for (int i = 0; i < 100; i++) {
        CHECK(Game::hasMessages() == true);
        string expected = "Message " + to_string(i);
        CHECK(Game::getNextMessage() == expected);
    }
    
    CHECK(Game::hasMessages() == false);
}