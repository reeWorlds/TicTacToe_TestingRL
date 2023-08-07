#include "TD3Player.h"

TD3Player::QNode::QNode()
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = 0.0f;
	}
}

TD3Player::TD3Player() : rng(chrono::high_resolution_clock::now().time_since_epoch().count())
{
	Q = nullptr;
}

TD3Player::~TD3Player()
{
	if (Q != nullptr)
	{
		delete[] Q;
	}
}

void TD3Player::readQ()
{
	ifstream in("saves/TD3Player.txt", ios::binary);

	Q = new QNode[N_States];

	in.read((char*)Q, N_States * sizeof(QNode));

	in.close();
}

void TD3Player::writeQ()
{
	ofstream out("saves/TD3Player.txt", ios::binary);

	out.write((char*)Q, N_States * sizeof(QNode));

	out.close();
}

void TD3Player::playAndLearn(float epsilo, float alpha, float gamma)
{
	float gammas[4] = { 1.0f, gamma, gamma * gamma, gamma * gamma * gamma };

	vector <pair<int, int> > history; // hash, xy

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

			history.push_back({ hash, xy });
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

			history.push_back({ hash, bestXY });
		}

		side ^= 1;
	}

	float reward = matrix.checkState();

	for (int i = 1; i <= min(int(history.size()), 4); i++)
	{
		int hash = history[history.size() - i].first;
		int xy = history[history.size() - i].second;

		Q[hash].a[xy] = Q[hash].a[xy] + alpha * (reward * gammas[i - 1] - Q[hash].a[xy]);
	}

	for (int i = 5; i < history.size(); i++)
	{
		int side = (history.size() - i + 4) % 2;

		int hash = history[history.size() - i].first;
		int xy = history[history.size() - i].second;

		int hash2 = history[history.size() - i + 4].first;
		int xy2 = history[history.size() - i + 4].second;

		float h2Score = side == 0 ? -1e9 : 1e9;

		matrix.a[xy2 / 3][xy2 % 3] = 0;

		for (int j = 0; j < 9; j++)
		{
			if (matrix.a[j / 3][j % 3] == 0)
			{
				float score = Q[hash2].a[j];

				if (side == 0)
				{
					if (score > h2Score)
					{
						h2Score = score;
					}
				}
				else
				{
					if (score < h2Score)
					{
						h2Score = score;
					}
				}
			}
		}

		Q[hash].a[xy] = Q[hash].a[xy] + alpha * (gammas[3] * h2Score - Q[hash].a[xy]);
	}
}

void TD3Player::initQ()
{
	Q = new QNode[N_States];

	const int N_Games = 1000000;

	for (int gameI = 0; gameI < N_Games; gameI++)
	{
		float epsilo = pow(1.0 - double(gameI) / N_Games, 2.5);
		float alpha = 0.1 - 0.09 * gameI / N_Games;

		playAndLearn(epsilo, alpha, 0.95);
	}
}

void TD3Player::train()
{
	if (filesystem::exists("saves/TD3Player.txt"))
	{
		readQ();
	}
	else
	{
		initQ();
		writeQ();
	}
}

void TD3Player::makeMove(Matrix& matrix, int side)
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