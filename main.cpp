#include <iostream>
#include <stdio.h>
#include "turn.cpp"
#include "vector"
#include "string.h"
#include <memory>
#include "utils/CppRandom.hpp"
#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>


bool nameAlreadyUsed (std::shared_ptr<Game> game, std::string name, bool output) {

	output = false;

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		if (name == game->alivePlayers->at(i)->name) {
			output = true;
		}
	}
	
	return output;

}

std::shared_ptr<Player> createPlayer (std::shared_ptr<Game> game, int role) {

	std::shared_ptr<Player> outputPlayer = std::make_shared<Player>();
	std::string name = "";

	std::cout << "Please enter a Name: ";
	do {
		std::cin >> name;
	} while (nameAlreadyUsed(game, name, true));

	outputPlayer->name = name;
	outputPlayer->role = role;
	outputPlayer->alive = true;
	outputPlayer->voteCounter = 0;
	//outputPlayer->currentGame = *game;

	std::cout << "You are a " << outputPlayer->showRole() << std::endl;
	std::cout << "Good Luck " << name << "!" << std::endl;

	std::string nothing;
	std::cout << std::endl << std::endl << "Do you know which role you are? (if yes, enter 'y' if not please read above)";
	std::cin >> nothing;
	system("clear");
	return outputPlayer;
}

std::shared_ptr<Player> createVillager (std::shared_ptr<Game> game) {
	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 1);
	game->alivePlayers->push_back(tmpPlayer);
	game->villagers->push_back(tmpPlayer);
	return tmpPlayer;
}

std::shared_ptr<Player> createWerewolf (std::shared_ptr<Game> game) {
	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 2);
	game->alivePlayers->push_back(tmpPlayer);
	game->werewolves->push_back(tmpPlayer);
	return tmpPlayer;
}

std::shared_ptr<Player> createSeer (std::shared_ptr<Game> game) {
	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 3);
	game->alivePlayers->push_back(tmpPlayer);
	game->seers->push_back(tmpPlayer);
	return tmpPlayer;
}

bool intInVector (int inputInt, std::vector<int> inputVector, bool output) {

	output = false;

	for (int i : inputVector) {
		if (inputInt == i) {
			output = true;
		}
	}
	return output;
}

std::shared_ptr<Game> roleDeployment(int playerCount) {
	std::shared_ptr<Game> outputGame = std::make_shared<Game>();

	std::vector<int> notVillagers;

	//Which players are werewolves
	int werewolveCount = playerCount / 4;
	std::vector<int> werewolves;
	while (werewolveCount > werewolves.size()) {
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, notVillagers, true) == false) {
			werewolves.push_back(tmpNumber);
			notVillagers.push_back(tmpNumber);
		}
	}

	//Which players are seers
	int seerCount = 0;
	if (playerCount > 4) {
		seerCount = 1;
	}
	std::vector<int> seers;
	while (seerCount > seers.size()) {
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, notVillagers, true) == false) {
			seers.push_back(tmpNumber);
			notVillagers.push_back(tmpNumber);
		}
	}
	

	//Creation of the players
	for (int i = 0; i < playerCount; i++)
	{
		std::shared_ptr<Player> tmpPlayer;
		if (intInVector(i, werewolves, true)) {
			tmpPlayer = createWerewolf(outputGame);
		} else if (intInVector(i, seers, true)) {
			tmpPlayer = createSeer(outputGame);
		} else {
			tmpPlayer = createVillager(outputGame);
		}

		tmpPlayer->id = i;

	}

	outputGame->werewolveCount = werewolveCount;

	return outputGame;
	
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //Game* mainGame = roleDeployment(playerCount);
	std::shared_ptr<Game> mainGame = roleDeployment(10);
	while (mainGame->gameOver == false) {
		turnNight(mainGame);
		if (mainGame->gameOver == false) {
			turnDay(mainGame);
		}
	}
	
}
