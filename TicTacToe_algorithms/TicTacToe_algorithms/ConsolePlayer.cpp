#include "ConsolePlayer.h"

void ConsolePlayer::train()
{

}

void ConsolePlayer::makeMove(Matrix& matrix, int side)
{
	char cmat[5][6] = {".|.|.", "-+-+-", ".|.|.", "-+-+-", ".|.|." };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cmat[i * 2][j * 2] = matrix.a[i][j] == 1 ? 'X' : matrix.a[i][j] == -1 ? 'O' : ' ';
		}
	}

	cout << "\n";
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << cmat[i][j];
		}
		cout << "\n";
	}
	cout << "Input x and y coordinates to place a " << (side == 0 ? "X" : "O") << ":\n";

	int x = -1, y = -1;
	while (!(x >= 0 && x < 3) || !(y >= 0 && y < 3) || matrix.a[x][y] != 0)
	{
		cin >> x >> y;
	}

	matrix.a[x][y] = side == 0 ? 1 : -1;
}