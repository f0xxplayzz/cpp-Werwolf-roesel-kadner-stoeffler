#pragma once
#include <stdio.h>
#include <string>

class Player {
	public:
		std::string name;
		std::string role = "";//Hilfsattribut, wird ben�tigt damit alle Unterklassen dies implementieren
		bool alive = true;
		int voteCounter;
		//bool isMayor;
		//Game currentGame;
		void voteExecution();
};