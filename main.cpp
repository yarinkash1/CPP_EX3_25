// yarinkash1@gmail.com
#include <iostream>
#include "coup.hpp"
#include <algorithm> // For shuffle
#include <random>    // For mt19937 and random_device
#include <chrono>    // For time functions
#include <thread>    // For sleep_for
#include <iomanip>   // For setw, setfill

enum cardType
{
    Governor = 0,
    Spy = 1,
    Baron = 2,
    General = 3,
    Judge = 4,
    Merchant = 5
};

string cardTypeToString(cardType type)
{
    switch (type)
    {
    case Governor:
        return "Governor";
    case Spy:
        return "Spy";
    case Baron:
        return "Baron";
    case General:
        return "General";
    case Judge:
        return "Judge";
    case Merchant:
        return "Merchant";
    default:
        return "Unknown";
    }
}

/**
 * @brief Initializes and returns a deck of cards with predefined types.
 *
 * This function creates a deck of cards of size `DECK_SIZE`, where each card
 * is assigned a type from the `cardType` enum in a repeating pattern (modulo 6).
 *
 * @param None
 * @return std::vector<cardType> A vector representing the initialized deck.
 * @throws None
 */
vector<cardType> initDeck()
{
    vector<cardType> deck;
    deck.reserve(DECK_SIZE);

    for (int i = 0; i < DECK_SIZE; i++)
    {
        deck.push_back(static_cast<cardType>(i % 6));
    }

    cout << "Initialized deck:" << endl;
    return deck;
}

/**
 * @brief Prints the contents of the deck to the standard output.
 *
 * This function iterates over the given deck and prints each card's index and
 * its corresponding string representation using `cardTypeToString`.
 *
 * @param deck A constant reference to a vector of `cardType` representing the deck to print.
 * @return void
 * @throws None
 */
void printDeck(const vector<cardType> &deck)
{
    cout << "Current deck:" << endl;
    for (size_t i = 0; i < deck.size(); i++)
    {
        cout << "#" << i + 1 << ": " << cardTypeToString(deck[i]) << endl;
    }
}

/**
 * @brief Randomly shuffles the order of cards in the deck.
 *
 * This function uses a Mersenne Twister pseudo-random generator (`mt19937`)
 * seeded with a random device to shuffle the deck in place using `std::shuffle`.
 *
 * @param deck A reference to a vector of `cardType` representing the deck to shuffle.
 * @return void
 * @throws None
 */
void shuffleDeck(vector<cardType> &deck)
{
    // Shuffle the deck
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

/**
 * @brief Displays a countdown timer in the format MM:SS.
 *
 * This function takes a number of minutes and seconds, calculates the total
 * seconds, and displays a countdown timer in the format MM:SS. The timer
 * updates every second until it reaches zero.
 *
 * @param minutes The number of minutes for the countdown.
 * @param seconds The number of seconds for the countdown.
 * @return void
 * @throws None
 */
void countdownTimer(int minutes, int seconds)
{
    int totalSeconds = minutes * 60 + seconds;

    for (int i = totalSeconds; i >= 0; --i)
    {
        int min = i / 60;
        int sec = i % 60;
        // \r is a carriage return. It is used to overwrite the current line in the console by moving the cursor back to the beginning of the line.
        // setw(2) sets the width of the output to 2 characters, and setfill('0') fills any empty space with '0'.
        // This is useful for displaying numbers with leading zeros, such as "01" instead of "1".
        // flush is used to ensure that the output is immediately displayed on the console.
        cout << "\r" << setw(2) << setfill('0') << min << ":"
             << setw(2) << setfill('0') << sec << flush;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "\nTime's up!" << endl;
}

int main()
{
    cout << "-- Demo started --" << endl;
    // vector<cardType> cards_deck = initDeck();
    // printDeck(cards_deck);
    // cout << "--------------------------------------------------------------------------------------------" << endl;
    // // Shuffle the deck
    // shuffleDeck(cards_deck);
    // printDeck(cards_deck);
    // cout << "--------------------------------------------------------------------------------------------" << endl;

    countdownTimer(1, 0); // 15 minutes and 0 seconds

    return 0;
}