#include "ConsolePlayer.h"
#include "DPPlayer.h"
#include "MonteCarloPlayer.h"
#include "QLearningPlayer.h"
#include "TD3Player.h"

#include "TicTacToeManager.h"



int main()
{
	//TicTacToeManager<ConsolePlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, DPPlayer> manager;
	//TicTacToeManager<MonteCarloPlayer, DPPlayer> manager;
	//TicTacToeManager<QLearningPlayer, DPPlayer> manager;
	TicTacToeManager<TD3Player, DPPlayer> manager;



	manager.trainPlayers();

	manager.startGame();


	// DQN
	// Double Dueling DQN
	// simple policy learning alg
	// PPO

	return 0;
}