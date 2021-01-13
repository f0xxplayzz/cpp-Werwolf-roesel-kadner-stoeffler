#pragma once
#include <iostream>
#include <stdio.h>
#include <memory>
#include <string.h>
//#include "./utils/CppRandom.hpp"

class Narrator {

	//Prints what the Narrator says. This could allow for random messages to appear.

	public:

		static void print (std::string s) {
			std::cout << s << std::endl;
		}

		static void gameStartVillage () {
			std::cout << "Good day dear residents of this village. You might have already noticed, that something is wrong in your village, as you can't remember the faces of anyone around you. "
				<< "The reason for that are werewolves, unfortunately since your lifestock has run away today, the werewolves will kill one of you every night. " 
				<< "Even though all hope seems lost, maybe there is a seeer among you, that can help you identify the werewolves." 
				<< std::endl;
		}

		static void gameStartWerewolve () {
			std::cout << "Good Morning werewolves, up until now you could survive with the livestock of the village, but it ran away today. "
				<< "Luckily you were able to get rid of most of the memories of the villagers, so you will survive for now. You are able to kill a villager every night. "
				<< "You have to be careful, there is a rumor about a seeer that has visited the village."
				<< std::endl;
		}

		static void dayStart (std::vector<std::string> names) {
			if (names.size() == 0) {
				std::cout << "Good Morning to all Residents, there haven't been any deaths last Round" << std::endl;
			} else {
				std::string s = "Good Morning to all Residents, we have to bid farewell to ";
				for (int i = 0; i < names.size() - 2; i++) {
					s += names[i] + ", ";
				}
				if (names.size() <= 2) {
					s+= names[names.size() - 1] + " and ";
				}
				s += names[names.size()] + ".";

				print(s);

			}
			
		}

		static void dayEndVillager () {
			std::cout << "Good Night to all Players, let us hope, that this night will be uneventful." << std::endl;
		}

		static void dayEndWerewolve () {
			std::cout << "You have survived the day, soon you will fall transform again." << std::endl;
		}

		static void werewolfNightStart () {
			std::cout << "The full moon can be seen, you have transformed into a werewolve." << std::endl;
		}
	
		static void deathsKilled (std::shared_ptr<std::vector<std::string>> names) {
			for (int i = 0; i < names->size(); i++) {
				std::cout << " Unfortunately " << names->at(i) << " has been murdered." << std::endl;
			}
		}

		static void deathsHung (std::shared_ptr<std::vector<std::string>> names) {
			for (int i = 0; i < names->size(); i++) {
				std::cout << names->at(i) << " has been hung." << std::endl;
			}
		}

		static void seeerNightStart () {
			std::cout << "You have awoken from your slumber." << std::endl;
		}

		static void voteExecutionPossibilities () {
			std::cout << "You can vote for the execution of one of the following players:" << std::endl;
		}

		static void voteExecutionChoice() {
			std::cout << "Which player do you want to vote? ";
		}

		static void showOtherWerewolvesStart() {
			std::cout << "The other werewolves are: ";
		}

		static void showOtherWerewolves(std::string werewolve) {
			std::cout << werewolve << " ";
		}

		static void voteKillStart() {
			std::cout << "You can vote to kill one of the following players:" << std::endl;
		}

		static void voteKillPossibilities(std::string werewolve, int iterator) {
			std::cout << iterator << ".) " << werewolve << std::endl;
		}

		static void voteKillDecision() {
			std::cout << "Who do you want to kill?" << std::endl;
		}

		static void voteSeerStart() {
			std::cout << "You can see the role of one of the following players:" << std::endl;
		}

		static void voteSeerPossibilities(std::string name, int iterator) {
			std::cout << iterator << ".) " << name << std::endl;
		}

		static void voteSeerDecision() {
			std::cout << "Which player do you want to investigate? ";
		}

		static void voteSeerResult(std::string name, std::string role) {
			std::cout << name << " is a " << role << "!" << std::endl;
		}

		static void gameEndVillager() {
			std::cout << "The Villagers have won!" << std::endl;
		}
		
		static void gameEndWerewolves() {
			std::cout << "The Werewolves have won!" << std::endl;
		}

		static void gameEndDraw() {
			std::cout << "The Narrator has won!" << std::endl;
		}

		static void wakeUp(std::string name) {
			std::cout << name << " has awoken from their slumber." << std::endl;
		}

		static void fallAsleep(std::string name) {
			std::cout << name << " has fallen asleep." << std::endl;
		}

		static void turnStart(std::string name) {
			std::cout << name << "s turn has started." << std::endl;
		}

		static void turnEnd() {
			std::cout << "Your turn has ended" << std::endl;
		}

		static void characterCreationNameSelect() {
			std::cout << "Please enter a Name: ";
		}

		static void characterRoleReturn(std::string name, std::string role) {
			std::cout << "You are a " << role << std::endl;
			std::cout << "Good Luck " << name << "!" << std::endl;
		}

};
