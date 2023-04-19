#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937



class Card
{
public:
	enum Suit
	{
	    club,
	    diamond,
	    heart,
	    spade,
	
	    max_suits
	};
	
	enum Rank
	{
	    rank_2,
	    rank_3,
	    rank_4,
	    rank_5,
	    rank_6,
	    rank_7,
	    rank_8,
	    rank_9,
	    rank_10,
	    rank_jack,
	    rank_queen,
	    rank_king,
	    rank_ace,
	
	    max_ranks
	};
private:
	
	Rank m_rank{};
	Suit m_suit{};


public:
	Card() = default;
	Card(Rank rank, Suit suit);
	~Card();
	void print() const;
	int value() const;


};

Card::Card(Rank rank, Suit suit)
	: m_rank(rank), m_suit(suit)
{
}

Card::~Card()
{
}

void Card::print() const
{
	    switch (m_rank)
	    {
	    case Rank::rank_2:      std::cout << '2';   break;
	    case Rank::rank_3:      std::cout << '3';   break;
	    case Rank::rank_4:      std::cout << '4';   break;
	    case Rank::rank_5:      std::cout << '5';   break;
	    case Rank::rank_6:      std::cout << '6';   break;
	    case Rank::rank_7:      std::cout << '7';   break;
	    case Rank::rank_8:      std::cout << '8';   break;
	    case Rank::rank_9:      std::cout << '9';   break;
	    case Rank::rank_10:     std::cout << 'T';   break;
	    case Rank::rank_jack:   std::cout << 'J';   break;
	    case Rank::rank_queen:  std::cout << 'Q';   break;
	    case Rank::rank_king:   std::cout << 'K';   break;
	    case Rank::rank_ace:    std::cout << 'A';   break;
	    default:
	        std::cout << '?';
	        break;
	    }
	
	    switch (m_suit)
	    {
	    case Suit::club:       std::cout << 'C';   break;
	    case Suit::diamond:    std::cout << 'D';   break;
	    case Suit::heart:      std::cout << 'H';   break;
	    case Suit::spade:      std::cout << 'S';   break;
	    default:
	        std::cout << '?';
	        break;
	    }
}

int Card::value() const
{
	    switch (m_rank)
    {
    case Rank::rank_2:        return 2;
    case Rank::rank_3:        return 3;
    case Rank::rank_4:        return 4;
    case Rank::rank_5:        return 5;
    case Rank::rank_6:        return 6;
    case Rank::rank_7:        return 7;
    case Rank::rank_8:        return 8;
    case Rank::rank_9:        return 9;
    case Rank::rank_10:       return 10;
    case Rank::rank_jack:     return 10;
    case Rank::rank_queen:    return 10;
    case Rank::rank_king:     return 10;
    case Rank::rank_ace:      return 11;
    default:
        assert(false && "should never happen");
        return 0;
    }
}


class Deck
{
private:
	std::array<Card,52> m_deck;
	int m_cardIndex{0};

public:
	Deck();
	~Deck();
	void print()const;
	void shuffle();
	const Card& dealCard();


};

Deck::Deck()
{
	int Deckpos = 0;
	for (int i = 0; i < Card::max_suits; i++)
	{
		for (int j = 0; j < Card::max_ranks; j++)
		{
			m_deck[Deckpos] = Card((Card::Rank)j, (Card::Suit)i);
			Deckpos++;
		}
		
	}
}

Deck::~Deck()
{
}

void Deck::print() const
{
	for (const auto& card : m_deck)
	{
		card.print();
		std::cout << " ";
	}
	std::cout << std::endl;
}

void Deck::shuffle()
{
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

	std::shuffle(m_deck.begin(), m_deck.end(), mt);
	m_cardIndex = 0;
}

const Card& Deck::dealCard()
{
	assert(m_cardIndex < m_deck.size() && "Ran out of cards in the deck");

	return m_deck[m_cardIndex++];
}

class Player
{
private:
	int m_value{ 0 };
	const static int g_maximumScore{ 21 };


public:
	Player();
	bool isBust();
	int drawCard(Deck& deck);
	int score();
	bool playerTurn(Deck deck);
	bool playerWantsHit();
	bool dealerWantsHit(Deck deck);


};

Player::Player()
{
}


bool Player::isBust()
{
	return (m_value > g_maximumScore);
}

int Player::drawCard(Deck& deck)
{
	int thecard = deck.dealCard().value();
	m_value += thecard;
	return thecard;
}

int Player::score()
{
	return m_value;
}



bool Player::playerWantsHit()
{
	while (true)
	{
		std::cout << "(h) to hit, or (s) to stand: ";

		char ch{};
		std::cin >> ch;

		switch (ch)
		{
		case 'h':
			return true;
		case 's':
			return false;
		}
	}
}

bool Player::dealerWantsHit(Deck deck)
{
	while (m_value < 17)
	{
		int cardValue = deck.dealCard().value();
		m_value += cardValue;
		std::cout << "The Dealer drew a " << cardValue << " for a total of " << m_value << std::endl;
	}
	return isBust();
}

bool Player::playerTurn(Deck deck)
{
	while (true)
	{
		if (m_value > g_maximumScore)
		{
			// This can happen even before the player had a choice if they drew 2
			// aces.
			std::cout << "You busted!\n";
			return true;
		}
		else
		{
			if (playerWantsHit())
			{
				int cardValue{ deck.dealCard().value() };
				m_value += cardValue;
				std::cout << "You were dealt a " << cardValue << " and now have " << m_value << std::endl;
			}
			else
			{
				// The player didn't go bust.
				return false;
			}
		}
	}
}

bool playBlackjack(Deck deck)
{
	while (true)
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


// Code from orignal blackjack before we put it into classes
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