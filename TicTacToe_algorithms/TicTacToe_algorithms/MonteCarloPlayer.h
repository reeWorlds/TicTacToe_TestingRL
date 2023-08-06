#pragma once

#include "Player.h"

class MonteCarloPlayer : public Player
{
	class QNode
	{
	public:

		float a[9];

		QNode();
	};

	mt19937 rng;

	QNode* Q;

	void readQ();
	void writeQ();

	void playAndLearn(float epsilo, float alpha);
	void initQ();

public:

	MonteCarloPlayer();
	~MonteCarloPlayer();

	void train() override;

	void makeMove(Matrix& matrix, int side) override;

};