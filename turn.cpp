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
	std::vector<Player> alivePlayers;
	std::vector<Villager> villagers;
	std::vector<Werewolve> werewolves;
	//std::vector<std::string> seeers;
	//std::vector<std::string> witches;
	//std::vector<std::string> deadPlayers;

	bool checkWinCondition(){

	}
};
class Player {
	public:
		std::string name;
		bool alive = true;
		int voteCounter;
		//bool isMayor;
		Game currentGame;
		void voteExecution();
};
void turnNight(Game g) {
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
	
	//checkWinCondition();
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
