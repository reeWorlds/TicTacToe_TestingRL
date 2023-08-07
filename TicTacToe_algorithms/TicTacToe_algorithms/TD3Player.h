#pragma once

#include "Player.h"

class TD3Player : public Player
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

	void playAndLearn(float epsilo, float alpha, float gamma);
	void initQ();

public:

	TD3Player();
	~TD3Player();

	void train() override;

	void makeMove(Matrix& matrix, int side) override;

};