#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include "Roles/villager.cpp"
#include "Roles/werewolves.cpp"
#include"narrator.cpp"
#include <iostream>
class Game {
	public:
	int werwolveCount;
	bool gameOver = false;
	std::vector<Player> alivePlayers;
	std::vector<Villager> villagers;
	std::vector<Werewolve> werewolves;
	//std::vector<std::string> seeers;
	//std::vector<std::string> witches;
	//std::vector<std::string> deadPlayers;

	void checkWinCondition() {
	/*
		Author: Jan Stöffler
		Description: Methode, welche überprüft ob ein Sieg erreicht ist
		Input: /
		Output: /
	*/
		if (villagers.size = 0) {
			//Gewinnbedingung für die Werwölfe: kein Dorfbewohner lebt mehr.
			std::cout << "Die Werwöfe haben gewonnen!" << std::endl;
			gameOver = true;
		}
		if (werewolves.size = 0) {
			//Gewinnbedingung für die Dorfbewohner: Alle Werwölfe sind tot.
			std::cout << "Die Dorfbewohner haben gewonnen!" << std::endl;
			gameOver = true;
		}
	}

	void executeWerewolveKill() {
	/*
		Author: Jan Stöffler
		Description: Methode, welche die Person tötet, die von den meisten Werwölfen gewählt wurde. Bei Gleichstand der Stimmen wird
			die zuerst im Vector genannte Person getötet.
		Input: /
		Output: /
	*/
		int mostVoted;
		for (int i = 0; i < villagers.size();i++) {
			int mostVotes = 0; //Merkvariable, daher innerhalb der Schleife
			if (villagers[i].voteCounter > mostVotes) {
				mostVotes = villagers[i].voteCounter;
				mostVoted = i;
			}
			villagers[i].voteCounter = 0;
		}
		villagers.erase(i);//löscht meist gevoteten Spieler aus dem vector Villagers
	}
};
class Player {
	public:
		std::string name;
		std::string role = "";//Hilfsattribut, wird benötigt damit alle Unterklassen dies implementieren
		bool alive = true;
		int voteCounter;
		//bool isMayor;
		Game currentGame;
		void voteExecution();
};
void turnNight(Game g) {
/*
	Author: Jan Stöffler
	Description: Methode, welche einen Nachtzyklus darstellt
	Input: Game g
	Output: /
*/
	//Narrator.startRound();
	//night{
	for(Player player: g.alivePlayers){
	std::cout<< player.name <<" wacht auf."<<std::endl;
	for(Werewolve w: g.werewolves){
			if(w.name==player.name){
			w.showOtherWereWolves(g.werewolves, player);
			g.villagers=w.voteKill(g.villagers);
			}
		}
		std::cout<<"Dein Zug ist vorbei"<<std::endl;
	} 
	
	g.checkWinCondition();
}

void turnDay() {
	/*for(all Players){
		p.voteExecution();
	}
	for(all Players){
		implement: find player with max votes
		->p.alive=false;
	}
	*/
	//checkWinCondition();
}
