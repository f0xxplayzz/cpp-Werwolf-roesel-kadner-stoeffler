#include <iostream>
#include <stdio.h>
#include "turn.cpp"
#include "vector"
#include "string.h"
#include <memory>
#include "utils/CppRandom.hpp"
#include "utils/VectorUtils.hpp"

bool nameAlreadyUsed (std::shared_ptr<Game> game, std::string name, bool output) {

	//checks whether the given name is already used by another Player.
	//This function should only be used before anyone can die.

	output = false;

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		if (name == game->alivePlayers->at(i)->name) {
			output = true;
		}
	}
	
	return output;

}

std::shared_ptr<Player> createPlayer (std::shared_ptr<Game> game, int role) {

	//Creates an object for a new Player, it needs to know which "role" should be assigned.
	
	Narrator nrt;

	std::shared_ptr<Player> outputPlayer = std::make_shared<Player>();
	std::string name = "";

	nrt.characterCreationNameSelect();
	do {
		std::cin >> name; //can't create another Player with the same name in 1 game
	} while (nameAlreadyUsed(game, name, true));

	outputPlayer->name = name;
	outputPlayer->role = role;
	outputPlayer->alive = true;
	outputPlayer->voteCounter = 0;

	nrt.characterRoleReturn(outputPlayer->name, outputPlayer->showRole());

	std::string nothing;
	std::cout << std::endl << std::endl << "Do you know which role you are? (if yes, enter 'y' if not please read above)";
	do {
		std::cin >> nothing;
	} while (nothing != "y");
	system("clear");
	return outputPlayer;
}

std::shared_ptr<Player> createVillager (std::shared_ptr<Game> game) {
	//Adds the Pointer to the correct vectors in "game"

	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 1);
	game->alivePlayers->push_back(tmpPlayer);
	game->villagers->push_back(tmpPlayer);
	return tmpPlayer;
}

std::shared_ptr<Player> createWerewolf (std::shared_ptr<Game> game) {
	//see at CreateVillager

	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 2);
	game->alivePlayers->push_back(tmpPlayer);
	game->werewolves->push_back(tmpPlayer);
	return tmpPlayer;
}

std::shared_ptr<Player> createSeer (std::shared_ptr<Game> game) {
	//see at CreateVillager

	std::shared_ptr<Player> tmpPlayer = createPlayer(game, 3);
	game->alivePlayers->push_back(tmpPlayer);
	game->seers->push_back(tmpPlayer);
	return tmpPlayer;
}

std::shared_ptr<Game> roleDeployment(int playerCount) {
	//Creates x Players and gives them the correct amount of roles where x is "playerCount"

	std::shared_ptr<Game> outputGame = std::make_shared<Game>();

	std::vector<int> notVillagers; //all Players, that should not be created as Villagers.

	//Which players are werewolves
	int werewolveCount = playerCount / 4; //acceptable amount of werewolves in relation to the players
	std::vector<int> werewolves;
	while (werewolveCount > werewolves.size()) {
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, notVillagers, true) == false) {
			werewolves.push_back(tmpNumber);
			notVillagers.push_back(tmpNumber);
		}
	}

	//Which players are seers
	//currently only 1 Player is a seer when there are more than 4 Players. There shouldn't be a problem to create more than 1 seer but this was never tested
	//with more Players you could make a new seer every 20 or so Players.
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
	

	//Creation of the players with their respective roles. 
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
		tmpPlayer->id = i; //A different id is given to each player
	}

	outputGame->werewolveCount = werewolveCount;

	return outputGame;
	
}
int main()
{
	//(re-)starts the game for Hot-chair
	
	Narrator nrt;
	std::string answer = "y";
	do {
		std::shared_ptr<Game> mainGame = roleDeployment(10);
		nrt.gameStartVillage();
		while (mainGame->gameOver == false) {
			turnNight(mainGame);
			if (mainGame->gameOver == false) {
				turnDay(mainGame);
			}
		}
		std::cout << "Do you want to play again? (Enter 'y' for yes otherwise enter anything else) "; 
		std::cin >> answer;
	} while (answer == "y");
	
}