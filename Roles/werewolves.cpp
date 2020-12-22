#pragma once
#include <stdio.h>
#include "./../turn.cpp"

class Werewolve : Player {
	void voteKill(std::vector<Villager>){
		
	}
	void showOtherWerewolves(std::vector<Werewolve> werewolves, Player p){
		std::cout<<"Die anderen Werwölfe sind: ";
		for(Werewolve w : werewolves){
			if(!p.name==w.name){
				std::cout<<name<<" ";
			}
		}
		std::cout<<std::endl;
	}
};
