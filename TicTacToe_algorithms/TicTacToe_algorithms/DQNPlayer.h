#pragma once

#include "Player.h"


class DQNPlayer : public Player
{
	class NN
	{
		float input[10];
		float l1[32];
		float l2[16];
		float output[9];

		float l1_w[32][18];
		float l2_w[16][32];
		float output_w[9][16];
		float l1_b[32];
		float l2_b[16];

		float l1_d[32];
		float l2_d[16];
		float output_d[9];
		float l1_dw[32][18];
		float l2_dw[16][32];
		float output_dw[9][16];
		float l1_db[32];
		float l2_db[16];

		void fillZeroForward();

		void fillZeroBackward();

		static float relu(float x);
		static float relu_prime(float y);

	public:

		void readWeights();
		void writeWeights();

		void initWeights(mt19937& rng);

		void forward(Matrix& matrix, float Qs[9]);

		void forwardBackward(Matrix& matrix, int actionI, float Qi_gt, float lr);
	};

	mt19937 rng;

	NN* nn;

	void playAndUpdateNN(float epsilon1, float epsilon2, float alpha);
	void trainNN();

	void applySoftmax(vector<pair<float, int> >& prbs);

public:

	DQNPlayer();
	~DQNPlayer();

	void train() override;

	void makeMove(Matrix& matrix, int side) override;

};