#pragma once
#include <iostream>
#include <cassert>
#include <algorithm>
#include <random>
#include <array>
#include "Card.h"



class Deck
{
private:
	std::array<Card, 52> m_deck;
	int m_cardIndex{ 0 };

public:
	Deck();
	~Deck();
	void print()const;
	void shuffle();
	const Card& dealCard();


};