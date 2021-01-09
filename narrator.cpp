#pragma once
#include <iostream>
#include <stdio.h>
#include "turn.cpp"
#include <memory>
#include <string.h>
#include "./utils/CppRandom.hpp"

class Narrator {

	//all std::cout static methods, maybe with random elements

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

		static void seeerNightStart () {
			std::cout << "You have awoken from your slumber." << std::endl;
		}

		static void voteExecutionPossibilities () {
			std::cout << "You can vote for the execution of one of the following players:" << std::endl;
		}

		static void voteExecutionChoice() {
			std::cout << "Which player do you want to vote? ";
		}

		static void showOtherWerewolves(std::shared_ptr<std::vector<std::string>> otherWerewolves) {
			std::cout << "The other werewolves are: ";
			for (int i = 0; i < otherWerewolves->size(); i++) {
				std::cout << otherWerewolves->at(i) << " ";
			}
			std::cout<<std::endl;
		}

		static void voteKillPossibilities(std::shared_ptr<std::vector<std::string>> aliveNamesWithoutWerewolves) {
			std::cout << "You can vote to kill one of the following players:" << std::endl;
			
		}
};