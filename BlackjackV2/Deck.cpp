#include "Deck.h"

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
	std::cout << "Deck Destroyed" << std::endl;
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