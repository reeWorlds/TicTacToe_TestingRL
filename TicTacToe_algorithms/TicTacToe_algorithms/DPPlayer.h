#pragma once

#include "Player.h"

class DPPlayer : public Player
{
	mt19937 rng;

	char* dp;

	void readDP();
	void writeDP();

	int initDP_inner(int hash, int side);
	void initDP();

public:

	DPPlayer();
	~DPPlayer();

	void train() override;

	void makeMove(Matrix& matrix, int side) override;

};