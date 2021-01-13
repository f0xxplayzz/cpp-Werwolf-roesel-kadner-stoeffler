#pragma once

#include <string>
#include <string.h>
#include "Definitions.hpp"
#include "./../utils/CppRandom.hpp"
#include "./../turn.cpp"

char* to_cString(std::string str)
{
    char* res = new char[str.length()+1];
    strcpy(res,str.c_str());
    return res;
}
std::string roleAsString(char c)
{
    /*
        converts the role-char to a std::string
        need for output
    */
    std::string role_str = "";
    switch(c)
    {
        case VILLAGER_ROLE:
        role_str="Villager";
        break;
        case WEREWOLVE_ROLE:
        role_str="Werewolve";
        break;
        case SEER_ROLE:
        role_str="Seer";
        break;
        case WITCH_ROLE:
        role_str="Witch";
        break;
    }
    return role_str;
}

std::vector<char> roleDeploymentNetwork(int playerCount) {
	std::vector<char> output;
	std::vector<int> notVillagers; //all Players, that should not be created as Villagers.

	int werewolveCount = playerCount / 4;
	int seerCount = 0;

	std::vector<int> werewolves;
	std::vector<int> seers;

	if (playerCount > 4) {
		seerCount = 1;
	}

	for (int i = 0; i < werewolveCount; i++)
	{
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, notVillagers, true) == false) {
			werewolves.push_back(tmpNumber);
			notVillagers.push_back(tmpNumber);
		} else {
			i--;
		}
	}

	for (int i = 0; i < seerCount; i++)
	{
		int tmpNumber = GetRandomNumberBetween(0, playerCount - 1);

		if (intInVector(tmpNumber, notVillagers, true) == false) {
			seers.push_back(tmpNumber);
			notVillagers.push_back(tmpNumber);
		} else {
			i--;
		}
	}

	for (int i = 0; i < playerCount; i++) {
		if (!intInVector(i, notVillagers, true)) {
			output.push_back(VILLAGER_ROLE);
		} else if (intInVector(i, werewolves, true)) {
			output.push_back(WEREWOLVE_ROLE);
		} else if (intInVector(i, seers, true)) {
			output.push_back(SEER_ROLE);
		} else {
			//THIS CASE SHOULD NOT HAPPEN
			output.push_back('9');
		}
	}

	return output;
}