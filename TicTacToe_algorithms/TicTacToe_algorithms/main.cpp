#include "ConsolePlayer.h"
#include "DPPlayer.h"
#include "MonteCarloPlayer.h"

#include "TicTacToeManager.h"



int main()
{
	//TicTacToeManager<ConsolePlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, DPPlayer> manager;

	TicTacToeManager<MonteCarloPlayer, DPPlayer> manager;

	manager.trainPlayers();

	manager.startGame();



	// Q-learning (TD(0))
	// kind of TD(2) (add gamma)
	// DQN
	// Double Dueling DQN
	// simple policy learning alg
	// PPO

	return 0;
}