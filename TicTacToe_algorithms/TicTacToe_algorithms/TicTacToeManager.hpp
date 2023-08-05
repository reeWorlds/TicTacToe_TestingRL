#include "TicTacToeManager.h"

template<class P1, class P2>
TicTacToeManager<P1, P2>::TicTacToeManager()
{
	p1 = new P1();
	p2 = new P2();
}

template<class P1, class P2>
void TicTacToeManager<P1, P2>::trainPlayers()
{
	p1->train();
	p2->train();
}

template<class P1, class P2>
void TicTacToeManager<P1, P2>::startGame()
{
	matrix.initZero();

	int side = 0;

	while (!matrix.isFinished())
	{
		if (side == 0)
		{
			p1->makeMove(matrix, side);
		}
		else
		{
			p2->makeMove(matrix, side);
		}

		side ^= 1;
	}

	int result = matrix.checkState();

	if (result == 0)
	{
		cout << "Draw!\n";
	}
	else if (result == 1)
	{
		cout << "X wins!\n";
	}
	else
	{
		cout << "O wins!\n";
	}
}