#include "TicTacToeBasics.h"

Matrix::Matrix()
{
	initZero();
}

Matrix::Matrix(int hash)
{
	initByHash(hash);
}

Matrix::Matrix(const Matrix& other)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			a[i][j] = other.a[i][j];
		}
	}
}

void Matrix::initZero()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			a[i][j] = 0;
		}
	}
}

void Matrix::initByHash(int hash)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			a[i][j] = hash % 3 - 1;
			hash /= 3;
		}
	}
}

int Matrix::isFinished()
{
	if (checkState() != 0)
	{
		return 1;
	}

	int sum = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			sum += abs(a[i][j]);
		}
	}

	return sum == 9;
}

int Matrix::checkState()
{
	for (int i = 0; i < 3; i++)
	{
		if (a[i][0] == a[i][1] && a[i][1] == a[i][2] && a[i][0] != 0)
		{
			return a[i][0];
		}

		if (a[0][i] == a[1][i] && a[1][i] == a[2][i] && a[0][i] != 0)
		{
			return a[0][i];
		}
	}

	if (a[0][0] == a[1][1] && a[1][1] == a[2][2] && a[0][0] != 0)
	{
		return a[0][0];
	}

	if (a[0][2] == a[1][1] && a[1][1] == a[2][0] && a[0][2] != 0)
	{
		return a[0][2];
	}

	return 0;
}

int Matrix::getHash()
{
	int hash = 0;
	
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 2; j >= 0; j--)
		{
			hash *= 3;
			hash += a[i][j] + 1;
		}
	}

	return hash;
}