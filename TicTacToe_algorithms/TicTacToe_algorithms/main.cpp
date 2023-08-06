#include "ConsolePlayer.h"
#include "DPPlayer.h"

#include "TicTacToeManager.h"



int main()
{
	//TicTacToeManager<ConsolePlayer, ConsolePlayer> manager;
	TicTacToeManager<DPPlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, DPPlayer> manager;

	manager.trainPlayers();

	manager.startGame();

	return 0;
}