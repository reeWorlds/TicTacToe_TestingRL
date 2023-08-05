#include "ConsolePlayer.h"

#include "TicTacToeManager.h"



int main()
{
	TicTacToeManager<ConsolePlayer, ConsolePlayer> manager;

	manager.trainPlayers();

	manager.startGame();



	return 0;
}