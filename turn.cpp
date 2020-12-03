
#include <stdio.h>
#include <vector>
#include <string>
#include "Roles/villager.cpp"
#include "Roles/werewolves.cpp"
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
void startTurn() {
	//narator(startRound);
	//night{
	/*for( all Players){
	if( p is werewolve){
		w.showOtherWereWolves();
		w.voteKill();
		}
	} 
	*/
	//}
	//day{
	/*for(all Players){
		p.voteExecution();
	}
	for(all Players){
		implement: find player with max votes
		->p.alive=false;
	}
	*/
	//}

}
