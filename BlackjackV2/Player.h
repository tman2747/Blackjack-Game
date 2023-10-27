#pragma once
#include <iostream>
#include "Deck.h"
#include "imgui.h"
#include "Logging.h"

class Player
{
private:
	int m_value{ 0 };
	const static int g_maximumScore{ 21 };


public:
	Player(Logging &logger);
	~Player();
	bool isBust();
	int drawCard(Deck& deck);
	int score();
	bool playerTurn(Deck deck);
	bool playerWantsHit();
	bool dealerWantsHit(Deck deck);
	void reset(Deck& deck, Player& dealer);

};