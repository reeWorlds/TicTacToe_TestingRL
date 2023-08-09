#include "ConsolePlayer.h"
#include "DPPlayer.h"
#include "MonteCarloPlayer.h"
#include "QLearningPlayer.h"
#include "TD3Player.h"
#include "DQNPlayer.h"

#include "TicTacToeManager.h"



int main()
{
	//TicTacToeManager<ConsolePlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, ConsolePlayer> manager;
	//TicTacToeManager<DPPlayer, DPPlayer> manager;
	//TicTacToeManager<MonteCarloPlayer, DPPlayer> manager;
	//TicTacToeManager<QLearningPlayer, DPPlayer> manager;
	//TicTacToeManager<TD3Player, DPPlayer> manager;


	for (int i = 0; i < 100; i++)
	{
		TicTacToeManager<DQNPlayer, DPPlayer> manager;

		manager.trainPlayers();

		manager.startGame();
	}
	for (int i = 0; i < 100; i++)
	{
		TicTacToeManager<DPPlayer, DQNPlayer> manager;

		manager.trainPlayers();

		manager.startGame();
	}


	// DQN (the very simplest version)
	// Double Dueling DQN (history of games)
	// simple policy learning alg ???
	// PPO

	return 0;
}