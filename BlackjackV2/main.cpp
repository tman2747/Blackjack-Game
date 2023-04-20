#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937
#include "Card.h"
#include "Deck.h"
#include "Player.h"


bool playBlackjack(Deck deck)
{
		Player player{};
		Player dealer{};
		
		    // Index of the card that will be drawn next. This cannot overrun
		    // the array, because a player will lose before all cards are used up.
		
		    // Create the dealer and give them 1 card.
		dealer.drawCard(deck);
		
		    // The dealer's card is face up, the player can see it.
		std::cout << "The dealer is showing: " << dealer.score() << '\n';
		
		    // Create the player and give them 2 cards.
		player.drawCard(deck);
		player.drawCard(deck);
		std::cout << "You have: " << player.score() << '\n';
		
		    if (player.playerTurn(deck))
		    {
		        // The player went bust.
		        return false;
		    }
		
			// maybe switch this to a if ? : thingy (its The conditional operator (or Ternary operator) chapter5.5)
		    if (dealer.dealerWantsHit(deck))
		    {
		        // The dealer went bust, the player wins.
		        return true;
		    }
		
		    return (player.score() > dealer.score());
}


bool playagain()
{
	std::cout << "would you like to play again ('y' for yes anything else for no)";
	char choice{};
	std::cin >> choice;


	if (choice == 'y' || choice == 'Y')
	{
		system("cls");
		return true;
	}

	return false;
}

int main()
{
	Deck deck{};


	// maybe switch this to a if ? : thingy (its The conditional operator (or Ternary operator) chapter5.5)
	do
	{
		deck.shuffle();

		if (playBlackjack(deck))
		{
			std::cout << "you win!" << std::endl;
		}
		else
		{
			std::cout << "dealer wins" << std::endl;
		}
	} while (playagain());


	return 0;
}


// *****Code from orignal blackjack before we put it into classes ****** 
//
//using Deck = std::array<Card, 52>;
//using Index = Deck::size_type;
//
//// Maximum score before losing.
//constexpr int g_maximumScore{ 21 };
//
//// Minimum score that the dealer has to have.
//constexpr int g_minimumDealerScore{ 17 };
//
//
//
//bool playerWantsHit()
//{
//    while (true)
//    {
//        std::cout << "(h) to hit, or (s) to stand: ";
//
//        char ch{};
//        std::cin >> ch;
//
//        switch (ch)
//        {
//        case 'h':
//            return true;
//        case 's':
//            return false;
//        }
//    }
//}
//
//// Returns true if the player went bust. False otherwise.
//bool playerTurn(const Deck& deck, Index& nextCardIndex, Player& player)
//{
//    while (true)
//    {
//        if (player.score > g_maximumScore)
//        {
//            // This can happen even before the player had a choice if they drew 2
//            // aces.
//            std::cout << "You busted!\n";
//            return true;
//        }
//        else
//        {
//            if (playerWantsHit())
//            {
//                int cardValue{ getCardValue(deck.at(nextCardIndex++)) };
//                player.score += cardValue;
//                std::cout << "You were dealt a " << cardValue << " and now have " << player.score << '\n';
//            }
//            else
//            {
//                // The player didn't go bust.
//                return false;
//            }
//        }
//    }
//}
//
//// Returns true if the dealer went bust. False otherwise.
//bool dealerTurn(const Deck& deck, Index& nextCardIndex, Player& dealer)
//{
//    // Draw cards until we reach the minimum value.
//    while (dealer.score < g_minimumDealerScore)
//    {
//        int cardValue{ getCardValue(deck.at(nextCardIndex++)) };
//        dealer.score += cardValue;
//        std::cout << "The dealer turned up a " << cardValue << " and now has " << dealer.score << '\n';
//
//    }
//
//    // If the dealer's score is too high, they went bust.
//    if (dealer.score > g_maximumScore)
//    {
//        std::cout << "The dealer busted!\n";
//        return true;
//    }
//
//    return false;
//}
//
//bool playBlackjack(const Deck& deck)
//{
//    // Index of the card that will be drawn next. This cannot overrun
//    // the array, because a player will lose before all cards are used up.
//    Index nextCardIndex{ 0 };
//
//    // Create the dealer and give them 1 card.
//    Player dealer{ getCardValue(deck.at(nextCardIndex++)) };
//
//    // The dealer's card is face up, the player can see it.
//    std::cout << "The dealer is showing: " << dealer.score << '\n';
//
//    // Create the player and give them 2 cards.
//    Player player{ getCardValue(deck.at(nextCardIndex)) + getCardValue(deck.at(nextCardIndex + 1)) };
//    nextCardIndex += 2;
//
//    std::cout << "You have: " << player.score << '\n';
//
//    if (playerTurn(deck, nextCardIndex, player))
//    {
//        // The player went bust.
//        return false;
//    }
//
//    if (dealerTurn(deck, nextCardIndex, dealer))
//    {
//        // The dealer went bust, the player wins.
//        return true;
//    }
//
//    return (player.score > dealer.score);
//}
//
//int main()
//{
//    auto deck{ createDeck() };
//
//    shuffleDeck(deck);
//
//    if (playBlackjack(deck))
//    {
//        std::cout << "You win!\n";
//    }
//    else
//    {
//        std::cout << "You lose!\n";
//    }
//
//    return 0;
//}