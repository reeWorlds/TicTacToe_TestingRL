#include "DPPlayer.h"

DPPlayer::DPPlayer() : rng(chrono::high_resolution_clock::now().time_since_epoch().count())
{
	dp = nullptr;
}

DPPlayer::~DPPlayer()
{
	if (dp != nullptr)
	{
		delete[] dp;
	}
}

void DPPlayer::readDP()
{
	ifstream in("saves/DPPlayer.txt", ios::binary);

	dp = new char[N_States];

	in.read(dp, N_States);

	in.close();
}

void DPPlayer::writeDP()
{
	ofstream out("saves/DPPlayer.txt", ios::binary);

	out.write(dp, N_States);

	out.close();
}

int DPPlayer::initDP_inner(int hash, int side)
{
	if (dp[hash] != 2)
	{
		return dp[hash];
	}

	Matrix matrix;
	matrix.initByHash(hash);

	int winner = matrix.checkState();

	if (winner != 0)
	{
		dp[hash] = winner;
		
		return dp[hash];
	}

	if (matrix.isFinished())
	{
		dp[hash] = 0;

		return dp[hash];
	}

	int val = side == 0 ? 1 : -1;

	vector <int> posDp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (matrix.a[i][j] == 0)
			{
				matrix.a[i][j] = val;
				posDp.push_back(initDP_inner(matrix.getHash(), side ^ 1));
				matrix.a[i][j] = 0;
			}
		}
	}

	int res;

	if (side == 0)
	{
		res = -1;
		for (auto it : posDp) { res = max(res, it); }
	}
	else
	{
		res = 1;
		for (auto it : posDp) { res = min(res, it); }
	}

	dp[hash] = res;

	return dp[hash];
}

void DPPlayer::initDP()
{
	dp = new char[N_States];

	for (int i = 0; i < N_States; i++)
	{
		dp[i] = 2;
	}

	int initState = 0;
	for (int i = 0; i < 9; i++)
	{
		initState *= 3;
		initState += 1;
	}

	initDP_inner(initState, 0);
}

void DPPlayer::train()
{
	if (filesystem::exists("saves/DPPlayer.txt"))
	{
		readDP();
	}
	else
	{
		initDP();
		writeDP();
	}
}

void DPPlayer::makeMove(Matrix& matrix, int side)
{
	Matrix mat(matrix);

	vector <pair<int, int> > moves;

	int val = side == 0 ? 1 : -1;
	int dpRes = dp[mat.getHash()];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (mat.a[i][j] == 0)
			{
				mat.a[i][j] = val;
				int tRes = dp[mat.getHash()];
				mat.a[i][j] = 0;

				if (tRes == dpRes)
				{
					moves.push_back({ i, j });
				}
			}
		}
	}
	
	int ri = rng() % moves.size();

	matrix.a[moves[ri].first][moves[ri].second] = val;
}