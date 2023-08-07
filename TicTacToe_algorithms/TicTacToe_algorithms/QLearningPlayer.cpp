#include "QLearningPlayer.h"

QLearningPlayer::QNode::QNode()
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = 0.0f;
	}
}

QLearningPlayer::QLearningPlayer() : rng(chrono::high_resolution_clock::now().time_since_epoch().count())
{
	Q = nullptr;
}

QLearningPlayer::~QLearningPlayer()
{
	if (Q != nullptr)
	{
		delete[] Q;
	}
}

void QLearningPlayer::readQ()
{
	ifstream in("saves/QLearningPlayer.txt", ios::binary);

	Q = new QNode[N_States];

	in.read((char*)Q, N_States * sizeof(QNode));

	in.close();
}

void QLearningPlayer::writeQ()
{
	ofstream out("saves/QLearningPlayer.txt", ios::binary);

	out.write((char*)Q, N_States * sizeof(QNode));

	out.close();
}

void QLearningPlayer::playAndLearn(float epsilo, float alpha)
{
	Matrix matrix;

	int side = 0;

	while (!matrix.isFinished())
	{
		int hash = matrix.getHash();

		float epsilo_s = (rng() % 1000000) / 1000000.0;

		int val = side == 0 ? 1 : -1, xy;

		if (epsilo_s < epsilo)
		{
			// choose random move
			vector <int> moves;

			for (int i = 0; i < 9; i++)
			{
				if (matrix.a[i / 3][i % 3] == 0)
				{
					moves.push_back(i);
				}
			}

			xy = moves[rng() % moves.size()];
		}
		else
		{
			// choose best move
			float bestScore = val * -1e9;

			for (int i = 0; i < 9; i++)
			{
				if (matrix.a[i / 3][i % 3] == 0)
				{
					float score = Q[hash].a[i];

					if (side == 0)
					{
						if (score > bestScore)
						{
							bestScore = score;
							xy = i;
						}
					}
					else
					{
						if (score < bestScore)
						{
							bestScore = score;
							xy = i;
						}
					}
				}
			}
		}

		matrix.a[xy / 3][xy % 3] = val;

		if (matrix.isFinished())
		{
			float reward = matrix.checkState();

			Q[hash].a[xy] = Q[hash].a[xy] + alpha * (reward - Q[hash].a[xy]);
		}
		else
		{
			int hash2 = matrix.getHash();

			float tdScore = val * 1e9;
			if (side == 0)
			{
				for (int i = 0; i < 9; i++)
				{
					if (matrix.a[i / 3][i % 3] == 0)
					{
						tdScore = min(tdScore, Q[hash2].a[i]);
					}
				}
			}
			else
			{
				for (int i = 0; i < 9; i++)
				{
					if (matrix.a[i / 3][i % 3] == 0)
					{
						tdScore = max(tdScore, Q[hash2].a[i]);
					}
				}
			}

			Q[hash].a[xy] = Q[hash].a[xy] + alpha * (tdScore - Q[hash].a[xy]);
		}

		side ^= 1;
	}
}

void QLearningPlayer::initQ()
{
	Q = new QNode[N_States];

	const int N_Games = 1000000;

	for (int gameI = 0; gameI < N_Games; gameI++)
	{
		float epsilo = pow(1.0 - double(gameI) / N_Games, 2.5);
		float alpha = 0.1 - 0.09 * gameI / N_Games;

		playAndLearn(epsilo, alpha);
	}
}

void QLearningPlayer::train()
{
	if (filesystem::exists("saves/QLearningPlayer.txt"))
	{
		readQ();
	}
	else
	{
		initQ();
		writeQ();
	}
}

void QLearningPlayer::makeMove(Matrix& matrix, int side)
{
	int val = side == 0 ? 1 : -1;
	int hash = matrix.getHash();

	float bestScore = val * -1e9;
	int bestXY;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (matrix.a[i][j] == 0)
			{
				float score = Q[hash].a[i * 3 + j];

				if (side == 0)
				{
					if (score > bestScore)
					{
						bestScore = score;
						bestXY = i * 3 + j;
					}
				}
				else
				{
					if (score < bestScore)
					{
						bestScore = score;
						bestXY = i * 3 + j;
					}
				}
			}
		}
	}

	matrix.a[bestXY / 3][bestXY % 3] = val;
}