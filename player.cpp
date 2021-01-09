#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "./../narrator.cpp"

class Player {
	public:
		int id;
		std::string name;
		int role ;
		/*
			1:Villager
			2:Werewolve
			3:Seer
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

	//UTILS
	bool intInVector (int inputInt, std::vector<int> inputVector, bool output) {

	output = false;

	for (int i : inputVector) {
		if (inputInt == i) {
			output = true;
		}
	}
	return output;
}

	//GENERAL
	std::string showRole() {
		if (role == 1) {
			return "Villager";
		} else if (role == 2) {
			return "Werewolve";
		} else if (role == 3) {
			return "Seer";
		} else {
			throw "roleNotImplemented";
		}	
	}

	void voteExecution(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers) {
		Narrator narrator;
		narrator.voteExecutionPossibilities();
		int ownNumber = 0;
		for (int i = 0; i < alivePlayers->size(); i++) {
			if (name != alivePlayers->at(i)->name) {
				std::cout << i+1 << ".) " << alivePlayers->at(i)->name << std::endl;
			} else {
				ownNumber = i;
			}
			
		}
		narrator.voteExecutionChoice();
		int vote = 0;
		do {
			std::cin >> vote;
			vote--;
		} while (!( (vote >= 0) && (vote < alivePlayers->size()) && vote != ownNumber) );

		alivePlayers->at(vote)->voteCounter++;
	}

	//WEREWOLVE
	void showOtherWerewolves(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers){
		if(role == 2){

			std::shared_ptr<std::vector<std::shared_ptr<Player>>> tmpList = std::make_shared<std::vector<std::shared_ptr<Player>>>();;
			
			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id && alivePlayers->at(i)->role == 2) {
					tmpList->push_back(alivePlayers->at(i));
				}
			}

			std::cout<<"Die anderen Werwölfe sind: ";

			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id && alivePlayers->at(i)->role == 2) {
					std::cout << alivePlayers->at(i)->name << " ";
				}
			}

			std::cout<<std::endl;
		}
	}
	void voteKill(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers){
		if(role==2){
			std::cout << "You can vote to kill one of the following players:" << std::endl;
			std::vector<int> werewolves;
			for (int i = 0; i < alivePlayers->size();i++) {
				if (alivePlayers->at(i)->role != 2) {
					std::cout << i+1 << ".) " << alivePlayers->at(i)->name << std::endl;
				} else {
					werewolves.push_back(i);
				}				
			}
			std::cout << "Who do you want to kill? ";
			int vote = 0;
			do {
				std::cin >> vote;
				vote--;
			} while (!( (vote >= 0) && (vote < alivePlayers->size()) && !intInVector(vote, werewolves, true)) );
			alivePlayers->at(vote)->voteCounter++;
		}
	}
	int voteKill_network(std::vector<Player> villagers){
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
			return villagers[vote - 1].id;
		}
	}

	int voteKill_network_with_shared_ptr(std::shared_ptr<std::vector<std::shared_ptr<Player>>> villagers){
		if(role==2){
			std::cout << "Folgende Leute kannst du töten:" << std::endl;
			int iterator = 1;
			for (int i = 0; i < villagers->size();i++) {
				std::cout << iterator << ".) " << villagers->at(i)->name << std::endl;
			}
			std::cout << "Wen willst du töten? ";
			int vote = 0;
			do {
			std::cin >> vote;
			} while (!(vote > 0) && !(vote <= villagers->size()));
			return villagers->at(vote - 1)->id;
		}
	}

	//SEER
	void voteVision(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers) {
		if (role == 3) {
			std::cout << "You can see the role of one of the following players:" << std::endl;
			int ownNumber = 0;
			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id) {
					std::cout << i+1 << ".) " << alivePlayers->at(i)->name << std::endl;
				} else {
					ownNumber = i;
				}
				
			}
			std::cout << "Which player do you want to investigate? ";
			int vote = 0;
			do {
				std::cin >> vote;
				vote--;
			} while (!( (vote >= 0) && (vote < alivePlayers->size()) && vote != ownNumber) );

			std::cout << alivePlayers->at(vote)->name << " is a " << alivePlayers->at(vote)->showRole() << "!" << std::endl;
		}
	}
};