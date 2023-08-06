#include "MonteCarloPlayer.h"

MonteCarloPlayer::QNode::QNode()
{
	for (auto& it : a)
	{
		it = 0.0;
	}
}

MonteCarloPlayer::MonteCarloPlayer() : rng(chrono::high_resolution_clock::now().time_since_epoch().count())
{
	Q = nullptr;
}

MonteCarloPlayer::~MonteCarloPlayer()
{
	if (Q != nullptr)
	{
		delete[] Q;
	}
}

void MonteCarloPlayer::readQ()
{
	ifstream in("saves/MonteCarloPlayer.txt", ios::binary);

	Q = new QNode[N_States];

	in.read((char*)Q, N_States * sizeof(QNode));

	in.close();
}

void MonteCarloPlayer::writeQ()
{
	ofstream out("saves/MonteCarloPlayer.txt", ios::binary);

	out.write((char*)Q, N_States * sizeof(QNode));

	out.close();
}

void MonteCarloPlayer::playAndLearn(float epsilo, float alpha)
{
	vector <int> history; // xy
	Matrix matrix;
	int side = 0;
	
	while (!matrix.isFinished())
	{
		int hash = matrix.getHash();

		float epsilo_s = (rng() % 1000000) / 1000000.0;

		int val = side == 0 ? 1 : -1;

		if (epsilo_s < epsilo)
		{
			// choose random move

			int arr[9], arrSz = 0;

			for (int xy = 0; xy < 9; xy++)
			{
				if (matrix.a[xy / 3][xy % 3] == 0)
				{
					arr[arrSz] = xy;
					arrSz++;
				}
			}

			int xy = arr[rng() % arrSz];
			matrix.a[xy / 3][xy % 3] = val;

			history.push_back(xy);
		}
		else
		{
			// choose move greedily

			float bestScore = val * -1e9;
			int bestXY;

			for (int xy = 0; xy < 9; xy++)
			{
				if (matrix.a[xy / 3][xy % 3] == 0)
				{
					float score = Q[hash].a[xy];

					if (side == 0)
					{
						if (score > bestScore)
						{
							bestScore = score;
							bestXY = xy;
						}
					}
					else
					{
						if (score < bestScore)
						{
							bestScore = score;
							bestXY = xy;
						}
					}
				}
			}

			matrix.a[bestXY / 3][bestXY % 3] = val;

			history.push_back(bestXY);
		}

		side ^= 1;
	}

	float reward = matrix.checkState();

	side = 0;
	matrix.initZero();

	for (auto xy : history)
	{
		int hash = matrix.getHash();

		int val = side == 0 ? 1 : -1;

		Q[hash].a[xy] += alpha * (reward - Q[hash].a[xy]);

		matrix.a[xy / 3][xy % 3] = val;

		side ^= 1;
	}
}

void MonteCarloPlayer::initQ()
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

void MonteCarloPlayer::train()
{
	if (filesystem::exists("saves/MonteCarloPlayer.txt"))
	{
		readQ();
	}
	else
	{
		initQ();
		writeQ();
	}
}

void MonteCarloPlayer::makeMove(Matrix& matrix, int side)
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