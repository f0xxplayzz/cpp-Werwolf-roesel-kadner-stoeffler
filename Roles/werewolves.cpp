#pragma once
#include "werewolves.hpp"

void Werewolves::Werewolve::voteKill(std::vector<Villager> villagers){
		std::cout << "Folgende Leute kannst du töten:" << std::endl;
		int iterator = 1;
		for (int i = 0; i < villagers.size();i++) {
			std::cout << iterator << ".) " << villagers[i].name << std::endl;
		}
		std::cout << "Wen willst du töten? ";
		int vote = 0;
		do {
		std::cin >> vote;
		} while (vote > 0 && vote <= villagers.size());
		villagers[vote - 1].voteCounter++;
}

void Werewolves::Werewolve::showOtherWerewolves(std::vector<Werewolves::Werewolve> werewolves, Player* p){
		std::cout<<"Die anderen Werwölfe sind: ";
		for(Werewolve w : werewolves){
			if(!(p->name==w.name)){
				std::cout<<w.name<<" ";
			}
		}
		std::cout<<std::endl;
}