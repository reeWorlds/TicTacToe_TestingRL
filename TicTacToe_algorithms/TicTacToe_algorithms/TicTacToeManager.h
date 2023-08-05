#pragma once

#include "Player.h"

template <class P1, class P2>
class TicTacToeManager
{
public:

	Player* p1, * p2;

	Matrix matrix;

	TicTacToeManager();

	void trainPlayers();

	void startGame();
};

#include "TicTacToeManager.hpp"