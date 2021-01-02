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
	int werewolveCount;
	bool gameOver = false;
	std::vector<Player> alivePlayers;
	std::vector<Villager> villagers;
	std::vector<Werewolves::Werewolve> werewolves;
	//std::vector<std::string> seeers;
	//std::vector<std::string> witches;
	//std::vector<std::string> deadPlayers;
	std::vector<std::string> diedThisCycle;

	void checkWinCondition() {
	/*
		Author: Jan St�ffler
		Description: Methode, welche �berpr�ft ob ein Sieg erreicht ist
		Input: /
		Output: /
	*/
		if (villagers.size() == werewolves.size()) {
			//Gewinnbedingung f�r die Werw�lfe: kein Dorfbewohner lebt mehr.
			std::cout << "Die Werw�fe haben gewonnen!" << std::endl;
			gameOver = true;
		}
		if (werewolves.size() == 0  && villagers.size()!=0) {
			//Gewinnbedingung f�r die Dorfbewohner: Alle Werw�lfe sind tot.
			std::cout << "Die Dorfbewohner haben gewonnen!" << std::endl;
			gameOver = true;
		}
		//Narrator win, toDO
	}

	void executeWerewolveKill() {
	/*
		Author: Jan St�ffler
		Description: Methode, welche die Person t�tet, die von den meisten Werw�lfen gew�hlt wurde. Bei Gleichstand der Stimmen wird
			die zuerst im Vector genannte Person get�tet.
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
		villagers.erase(villagers.begin()+mostVoted);//l�scht meist gevoteten Spieler aus dem vector Villagers
	}
	std::string toString(){
		std::string temp = "";
		temp += (char)werewolveCount;
		temp += (char)gameOver;
		std::string temp2 ="";
			for(Player p :alivePlayers)
			{
				temp2 += p.toString();
			}
		temp += temp2;
		temp2 = "";
			for(Villager v : villagers)
			{
				temp2 += v.toString();
			} 
		temp += temp2;
		temp2 = "";
			for(Werewolves::Werewolve w : werewolves)
			{
				temp2 += w.toString();
			} 
		temp += temp2;
	}
};

void turnNight(Game g) {
/*
	Author: Jan St�ffler
	Description: Methode, welche einen Nachtzyklus darstellt
	Input: Game g
	Output: /
*/
	//Narrator.startRound();
	//night{
	for(Player player: g.alivePlayers){
	std::cout<< player.name <<" wacht auf."<<std::endl;
	//input
		for(Werewolves::Werewolve w: g.werewolves){
			if(w.name==player.name){
				w.showOtherWerewolves(g.werewolves, &player);
				w.voteKill(g.villagers);
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
