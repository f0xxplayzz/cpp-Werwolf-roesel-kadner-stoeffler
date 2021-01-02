#pragma once
#include "Server.hpp"
#include "Client.hpp"

namespace Network{
    Game processPlayerInfo(std::string);
    std::vector<Player> processAlivePlayers(char* c);
    std::vector<Villager> processVillagers(char* c);
    std::vector<Werewolves::Werewolve> processWerewolves(char* c);
    void sendGameInfo(WerewolveServer::Connection);
}