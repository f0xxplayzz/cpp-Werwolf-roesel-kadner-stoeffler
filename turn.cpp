
#include <stdio.h>
#include <vector>
#include <string>
#include "Roles/villager.cpp"
#include "Roles/werewolves.cpp"
#include"narrator.cpp"
class Game {
	int werwolveCount;
	std::vector<Player> alivePlayers;
	std::vector<std::string> villagers;
	std::vector<std::string> werewolves;
	//std::vector<std::string> seeers;
	//std::vector<std::string> witches;
	//std::vector<std::string> deadPlayers;
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
void turnNight() {
	//Narrator.startRound();
	//night{
	/*for( all Players){
	if( p is werewolve){
		w.showOtherWereWolves();
		w.voteKill();
		}
	} 
	//std::cout<<"Dein Zug ist vorbei"<<std::endl;
	*/
	//}
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
