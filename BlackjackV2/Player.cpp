#include "Player.h"

Player::Player(Logging &logger)
{
}

Player::~Player()
{
	std::cout << "Player Destroyed" <<std::endl;
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
	// returns true if dealer bust false if not
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
void Player::reset(Deck& deck, Player& dealer)
{
	// resets player dealer and shuffles the deck.
	// (shuffling the deck also resets deck index pos)
	m_value = 0;
	dealer.m_value = 0;
	deck.shuffle();
}