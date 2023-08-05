#pragma once

#include "Player.h"

class ConsolePlayer : public Player
{
public:

	void train() override;

	void makeMove(Matrix& matrix, int side) override;

};