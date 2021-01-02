#pragma once
#include <stdio.h>
#include <string>

class Player {
	public:
		char id;
		std::string name;
		char role ;//Hilfsattribut, wird ben�tigt damit alle Unterklassen dies implementieren
		/*
			1:Villager
			2:Werewolve
		*/
		bool alive = true;
		int voteCounter;
		//bool isMayor;
		//Game currentGame;
		void voteExecution();
		std::string toString(){
			std::string temp = "";
			temp += name;
			temp += '?';
			char c = voteCounter;
			temp += c;

			return temp; 
		}
	void showOtherWerewolves(std::vector<Player> werewolves, Player* p){
		if(role == 2){
			std::cout<<"Die anderen Werwölfe sind: ";
			for(Player w : werewolves){
				if(!(p->name==w.name)){
					std::cout<<w.name<<" ";
				}
			}
			std::cout<<std::endl;
		}
	}
	void voteKill(std::vector<Player> villagers){
		if(role==2){
			std::cout << "Folgende Leute kannst du töten:" << std::endl;
			int iterator = 1;
			for (int i = 0; i < villagers.size();i++) {
				std::cout << iterator << ".) " << villagers[i].name << std::endl;
			}
			std::cout << "Wen willst du töten? ";
			int vote = 0;
			do {
			std::cin >> vote;
			} while (!(vote > 0) && !(vote <= villagers.size()));
			villagers[vote - 1].voteCounter++;
		}
	}
};