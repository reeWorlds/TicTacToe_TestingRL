#pragma once

#include "includes.h"
#include "TicTacToeBasics.h"

class Player
{
public:

	virtual void makeMove(Matrix& matrix, int side) = 0;
	virtual void train() = 0;

};