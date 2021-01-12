#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include "narrator.cpp"
#include <iostream>
#include "player.cpp"


class Game {
	//current Status of a game is saved in this class
	public:
	int werewolveCount;
	bool gameOver = false;

	std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> villagers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> werewolves = std::make_shared<std::vector<std::shared_ptr<Player>>>(); //Players are deleted from their respective vectors as well when they die.
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> seers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> witches = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> deadPlayers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> diedThisCycle = std::make_shared<std::vector<std::shared_ptr<Player>>>();

	void checkWinCondition() {

		//Checks whether a team has won/lost. If both teams are dead the narrator wins.

		if (villagers->size() + seers->size() == werewolves->size() && villagers->size() + seers->size() != 0) {
			//If the amount of werewolves is equal to the amount of villagers, then the werewolves win.
			Narrator nrt;
			nrt.gameEndWerewolves();
			gameOver = true;
		}
		if (werewolves->size() == 0  && (villagers->size() != 0 || !seers->empty())) {
			//If there are no werewolves anymore and someone from the villagers survived they win.
			Narrator nrt;
			nrt.gameEndVillager();
			gameOver = true;
		}
		if (werewolves->size() == 0 && villagers->size() == 0) {
			//If no one is left alive, then the Narrator wins.
			Narrator nrt;
			nrt.gameEndDraw();
			gameOver = true;
		}
		
	}

	void playerDeath(int id) {
		//A players death is handled by their id.
		//they are removed/added to the correct vectors.

		std::shared_ptr<Player> tmpPlayer;

		for (int i = 0; i < alivePlayers->size(); i++) {
			if (id == alivePlayers->at(i)->id) {
				tmpPlayer = alivePlayers->at(i);
			}
		}

		tmpPlayer->alive = false;
		deadPlayers->push_back(tmpPlayer);

		int role = tmpPlayer->role;

		for (int i = 0; i < alivePlayers->size(); i++) {
			if (tmpPlayer->id == alivePlayers->at(i)->id) {
				alivePlayers->erase(alivePlayers->begin() + i);
			}
		}

		int toDelete = 0;

		if (role == 1) {
			for (int i = 0; i < villagers->size(); i++) {
				if (id == villagers->at(i)->id) {
					toDelete = i;
				}
			}
			villagers->erase(alivePlayers->begin() + toDelete);
		}

		if (role == 2) {
			for (int i = 0; i < werewolves->size(); i++) {
				if (id == werewolves->at(i)->id) {
					toDelete = i;
				}
			}
			werewolves->erase(alivePlayers->begin() + toDelete);
		}
		if (role == 3) {
			for (int i = 0; i < seers->size(); i++) {
				if (id == seers->at(i)->id) {
					toDelete = i;
				}
			}
			seers->erase(alivePlayers->begin() + toDelete);
		}
		
	}

	void executeVotes() {
		//executes the Player with the most votes. If there is a tie, then the Player that was first added into the game dies.
		//All votes are reset at the same time.

		int mostVoted = 0;
		int mostVotes = 0;
		for (int i = 0; i < alivePlayers->size();i++) {
			if (alivePlayers->at(i)->voteCounter > mostVotes) {
				mostVotes = alivePlayers->at(i)->voteCounter;
				mostVoted = i;
			}
			alivePlayers->at(i)->voteCounter = 0;
		}
		playerDeath(alivePlayers->at(mostVoted)->id);
	}

};

void turnNight(std::shared_ptr<Game> game) {
	//A night is played through. All players wake up and do what they need to do. At least one Player will die.
	
	Narrator nrt;

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		nrt.wakeUp(game->alivePlayers->at(i)->name);

		if (game->alivePlayers->at(i)->role == 1) {

		} else if (game->alivePlayers->at(i)->role == 2) {
			game->alivePlayers->at(i)->showOtherWerewolves(game->alivePlayers);
			game->alivePlayers->at(i)->voteKill(game->alivePlayers);
		} else if (game->alivePlayers->at(i)->role == 3) {
			game->alivePlayers->at(i)->voteVision(game->alivePlayers);
		}

		nrt.fallAsleep(game->alivePlayers->at(i)->name);

		std::string nothing;
		std::cout << std::endl << std::endl << "Please enter 'y'";
		do {
			std::cin >> nothing;
		} while (nothing != "y");

		system("clear");
	}

	game->executeVotes();
	
	game->checkWinCondition();
}

void turnDay(std::shared_ptr<Game> game) {
	//All Players vote for the death of another Player.
	//At least 1 Player will die.
	Narrator nrt;

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		nrt.turnStart(game->alivePlayers->at(i)->name);
		game->alivePlayers->at(i)->voteExecution(game->alivePlayers);
		nrt.turnEnd();
		system("clear");
	}

	game->executeVotes();

	game->checkWinCondition();
	
}
