#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "narrator.cpp"
#include "utils/VectorUtils.hpp"


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

	//GENERAL
	std::string showRole() {
		if (role == 1) {
			return "Villager";
		} else if (role == 2) {
			return "Werewolve";
		} else if (role == 3) {
			return "Seer";
		} else {
			std::cout << "role not implemented in player.cpp::showRole" << std::endl;
			throw "roleNotImplemented";
		}	
	}

	std::string toString(){
			std::string temp = "";
			temp += name;
			temp += '?';
			char c = voteCounter;
			temp += c;

			return temp; 
		}

	void voteExecution(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers) {
		//prints the list of possible names and increases the vote-counter of the chosen Player.
		Narrator nrt;
		nrt.voteExecutionPossibilities();
		int ownNumber = 0;
		for (int i = 0; i < alivePlayers->size(); i++) {
			if (name != alivePlayers->at(i)->name) {
				nrt.voteKillPossibilities(alivePlayers->at(i)->name, i+1);
			} else {
				ownNumber = i;
			}
			
		}
		nrt.voteExecutionChoice();
		int vote = 0;
		do {
			std::cin >> vote;
			vote--;
		} while (!( (vote >= 0) && (vote < alivePlayers->size()) && vote != ownNumber) );

		alivePlayers->at(vote)->voteCounter++;
	}

	//WEREWOLVE
	void showOtherWerewolves(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers) {
		//Prints the other living Werewolves
		if(role == 2){

			Narrator nrt;

			std::shared_ptr<std::vector<std::shared_ptr<Player>>> tmpList = std::make_shared<std::vector<std::shared_ptr<Player>>>();;
			
			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id && alivePlayers->at(i)->role == 2) {
					tmpList->push_back(alivePlayers->at(i));
				}
			}

			nrt.showOtherWerewolvesStart();
			
			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id && alivePlayers->at(i)->role == 2) {
					nrt.showOtherWerewolves(alivePlayers->at(i)->name);
				}
			}

			std::cout<<std::endl;
		}
	}

	void voteKill(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers){
		//The Werewolve can vote to kill another Player, that is not a werewolve.
		if(role==2){
			Narrator nrt;
			nrt.voteKillStart();
			std::vector<int> werewolves;
			for (int i = 0; i < alivePlayers->size();i++) {
				if (alivePlayers->at(i)->role != 2) {
					nrt.voteKillPossibilities(alivePlayers->at(i)->name, i+1);
				} else {
					werewolves.push_back(i);
				}				
			}
			nrt.voteKillDecision();
			int vote = 0;
			do {
				std::cin >> vote;
				vote--;
			} while (!( (vote >= 0) && (vote < alivePlayers->size()) && !intInVector(vote, werewolves, true)) );
			alivePlayers->at(vote)->voteCounter++;
		}
	}

	//SEER
	void voteVision(std::shared_ptr<std::vector<std::shared_ptr<Player>>> alivePlayers) {
		//Night of the seer.
		if (role == 3) {
			Narrator nrt;
			nrt.voteSeerStart();
			int ownNumber = 0;
			for (int i = 0; i < alivePlayers->size(); i++) {
				if (id != alivePlayers->at(i)->id) {
					nrt.voteSeerPossibilities(alivePlayers->at(i)->name, i+1);
				} else {
					ownNumber = i;
				}
				
			}
			nrt.voteSeerDecision();
			int vote = 0;
			do {
				std::cin >> vote;
				vote--;
			} while (!( (vote >= 0) && (vote < alivePlayers->size()) && vote != ownNumber) );

			nrt.voteSeerResult(alivePlayers->at(vote)->name, alivePlayers->at(vote)->showRole());
			std::string nothing;
			std::cout << std::endl << std::endl << "Do you know which role they are? (if yes, enter 'y' if not please read above)";
			do {
				std::cin >> nothing;
			} while (nothing != "y");
		}
	}
};
