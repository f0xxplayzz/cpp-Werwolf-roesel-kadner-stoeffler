#pragma once
#include <stdio.h>
#include <string>

class Player {
	public:
		std::string name;
		char role ;//Hilfsattribut, wird ben�tigt damit alle Unterklassen dies implementieren
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