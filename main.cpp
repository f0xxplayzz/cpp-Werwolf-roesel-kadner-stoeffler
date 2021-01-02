#include <iostream>
#include <stdio.h>
#include "turn.cpp"
#include "vector"
#include "string.h"
#include <memory>
#include "CppRandom.hpp"
#include "./Roles/werewolves.cpp"
#include "./Roles/villager.cpp"

void createWerewolf (std::shared_ptr<Game> game) {
	std::shared_ptr<Werewolves::Werewolve> outputWerewolve = std::make_shared<Werewolves::Werewolve>();
	//Werewolve *outputWerewolve = new Werewolve;
	std::shared_ptr<Player> outputPlayer = std::make_shared<Player>();
	//Player *outputPlayer = new Player;
	std::string name = "";

	std::cout << "Please enter a Name:";
	std::cin >> name;

	std::cout << "You are a Werewolve" << std::endl;
	std::cout << "Good Luck " << name << "!" << std::endl;

	outputWerewolve->name = name;
	outputWerewolve->alive = true;
	outputWerewolve->voteCounter = 0;
	//outputWerewolve->currentGame = *game;

	outputPlayer->name = name;
	outputPlayer->role = "werewolve";
	outputPlayer->alive = true;
	outputPlayer->voteCounter = 0;
	//outputPlayer->currentGame = *game;

	game->alivePlayers.push_back(*outputPlayer);
	game->werewolves.push_back(*outputWerewolve);

	std::string nothing;
	std::cout << std::endl << std::endl << "Do you know which role you are? (if yes, enter 'y' if not please read above)";
	std::cin >> nothing;
	system("clear");
}

void createVillager (std::shared_ptr<Game> game) {
	std::shared_ptr<Villager> outputVillager = std::make_shared<Villager>();
	//Villager *outputVillager = new Villager;
	std::shared_ptr<Player> outputPlayer = std::make_shared<Player>();
	//Player *outputPlayer = new Player;
	std::string name = "";

	std::cout << "Please enter a Name:";
	std::cin >> name;

	std::cout << "You are a Villager" << std::endl;
	std::cout << "Good Luck " << name << "!" << std::endl;

	outputVillager->name = name;
	outputVillager->alive = true;
	outputVillager->voteCounter = 0;
	//outputVillager->currentGame = *game;

	outputPlayer->name = name;
	outputPlayer->role = "villager";
	outputPlayer->alive = true;
	outputPlayer->voteCounter = 0;
	//outputPlayer->currentGame = *game;

	game->alivePlayers.push_back(*outputPlayer);
	game->villagers.push_back(*outputVillager);

	std::string userTesting;
	std::cout << std::endl << std::endl << "Do you know which role you are? (if yes, enter 'y' if not please read above)";
	std::cin >> userTesting;
	while (userTesting != "y") {
		std::cout << std::endl << std::endl << "Do you know which role you are? (if yes enter 'y', if not please read above)" << std::endl;
		std::cin >> userTesting;
	}
	system("clear");
}

bool intInVector (int inputInt, std::vector<int> inputVector, bool output) {

	for (int i : inputVector) {
		if (inputInt == i) {
			return true;
		}
	}
	return false;
}

std::shared_ptr<Game> roleDeployment(int playerCount) {
	std::shared_ptr<Game> outputGame = std::make_shared<Game>();

	//Which players are werewolves
	int werewolveCount = playerCount / 4;
	std::vector<int> werewolves;
	while (werewolveCount > werewolves.size()) {
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, werewolves, true) == false) {
			werewolves.push_back(tmpNumber);
		}
	}

	//Creation of the players
	for (int i = 0; i < playerCount; i++)
	{
		if (intInVector(i, werewolves, true)) {
			createWerewolf(outputGame);
		} else {
			createVillager(outputGame);
		}
	}

	outputGame->werewolveCount = werewolveCount;

	return outputGame;
	
}
int main()
{
	//Game* mainGame = roleDeployment(playerCount);
	std::shared_ptr<Game> mainGame = roleDeployment(9);
	//turnNight(Game g);
	turnDay();
}