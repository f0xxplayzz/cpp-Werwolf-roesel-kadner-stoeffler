#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include "narrator.cpp"
#include <iostream>
#include "Roles/player.cpp"


class Game {
	public:
	int werewolveCount;
	bool gameOver = false;

	std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> villagers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> werewolves = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> seers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> witches = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> deadPlayers = std::make_shared<std::vector<std::shared_ptr<Player>>>();
	std::shared_ptr<std::vector<std::shared_ptr<Player>>> diedThisCycle = std::make_shared<std::vector<std::shared_ptr<Player>>>();

	void checkWinCondition() {
	/*
		Author: Jan St�ffler
		Description: Methode, welche �berpr�ft ob ein Sieg erreicht ist
		Input: /
		Output: /
	*/

		if (villagers->size() == werewolves->size() && villagers->size() != 0) {
			//Gewinnbedingung f�r die Werw�lfe: kein Dorfbewohner lebt mehr.
			std::cout << "The Werewolves have won!" << std::endl;
			gameOver = true;
		}
		if (werewolves->size() == 0  && (villagers->size()!=0 || !seers->empty())) {
			//Gewinnbedingung f�r die Dorfbewohner: Alle Werw�lfe sind tot.
			std::cout << "The Villagers have won!" << std::endl;
			gameOver = true;
		}
		if (werewolves->size() == 0 && villagers->size() == 0) {
			//Narrator win
			std::cout << "The Narrator has won!" << std::endl;
			gameOver = true;
		}
		
	}

	void resetVotes() {
		for (int i = 0; i < alivePlayers->size(); i++)
		{
			alivePlayers->at(i)->voteCounter = 0;
		}
		
	}

	void playerDeath(int id) {

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
	/*
		Author: Jan St�ffler
		Description: Methode, welche die Person t�tet, die von den meisten Werw�lfen gew�hlt wurde. Bei Gleichstand der Stimmen wird
			die zuerst im Vector genannte Person get�tet.
		Input: /
		Output: /
	*/
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
		//l�scht meist gevoteten Spieler aus dem vector Villagers
	}

	std::string toString(){
		std::string temp = "";
		temp += (char)werewolveCount;
		temp += (char)gameOver;
		std::string temp2 ="";
		for (int i = 0; i < alivePlayers->size(); i++) {
			temp2 += alivePlayers->at(i)->toString();
		}
		temp += temp2;
		temp2 = "";
		for (int i = 0; i < villagers->size(); i++) {
			temp2 += villagers->at(i)->toString();
		}
		temp += temp2;
		temp2 = "";
		for (int i = 0; i < werewolves->size(); i++) {
			temp2 += werewolves->at(i)->toString();
		}
		temp += temp2;
		/*temp2 = "";
		for (int i = 0; i < seers->size(); i++) {
			temp2 += seers->at(i)->toString();
		}
		temp += temp2;*/
		return temp;
	}
};

void turnNight(std::shared_ptr<Game> game) {
/*
	Author: Jan St�ffler
	Description: Methode, welche einen Nachtzyklus darstellt
	Input: Game g
	Output: /
*/
	//Narrator.startRound();
	//night{

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		std::cout << game->alivePlayers->at(i)->name << " wakes up." << std::endl;

		if (game->alivePlayers->at(i)->role == 1) {

		} else if (game->alivePlayers->at(i)->role == 2) {
			game->alivePlayers->at(i)->showOtherWerewolves(game->alivePlayers);
			game->alivePlayers->at(i)->voteKill(game->alivePlayers);
		} else if (game->alivePlayers->at(i)->role == 3) {
			game->alivePlayers->at(i)->voteVision(game->alivePlayers);
		}

		std::cout<<"Dein Zug ist vorbei"<<std::endl;
		system("clear");
	}

	game->executeVotes();
	
	game->checkWinCondition();
}

void turnDay(std::shared_ptr<Game> game) {

	for (int i = 0; i < game->alivePlayers->size(); i++) {
		std::cout << game->alivePlayers->at(i)->name << "s turn." << std::endl;
		game->alivePlayers->at(i)->voteExecution(game->alivePlayers);
		std::cout<<"Dein Zug ist vorbei"<<std::endl;
		system("clear");
	}

	game->executeVotes();

	game->checkWinCondition();
	
}
