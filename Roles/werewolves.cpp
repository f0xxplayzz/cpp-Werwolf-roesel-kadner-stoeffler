#pragma once
#include <iostream>
#include <stdio.h>
#include "./../turn.cpp"

class Werewolve : Player {
	std::vector<Villager> voteKill(std::vector<Villager> villagers){
		std::cout << "Folgende Leute kannst du töten:" << std::endl;
		int k = 1;
		for (int i = 0; i < villagers.size();i++) {
			std::cout << k << ".) " << villagers[i].name << std::endl;
		}
		std::cout << "Wen willst du töten? ";
		int vote = 1;
		do {
			std::stoi(std::cin) >> vote;
		} while (vote > 0 && vote <= villagers.size());
		erase(villagers, villagers[vote - 1]);
		return villagers;
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
