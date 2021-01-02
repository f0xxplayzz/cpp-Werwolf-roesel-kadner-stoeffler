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
};