#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include "player.cpp"
#include "villager.cpp"
namespace Werewolves{
class Werewolve : public Player {
public:
	std::string role = "Werewolve";

	void voteKill(std::vector<Villager> villagers);

	void showOtherWerewolves(std::vector<Werewolve> werewolves, Player* p);
	
};
}
