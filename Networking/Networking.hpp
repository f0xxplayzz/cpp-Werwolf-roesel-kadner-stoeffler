#pragma once
#include "Server.hpp"

namespace Network{
    Game processPlayerInfo(std::shared_ptr<WerewolveServer::Connection> c);
    std::vector<Player> processAlivePlayers(char* c);
    std::vector<Villager> processVillagers(char* c);
    std::vector<Werewolves::Werewolve> processWerewolves(char* c);
    std::string gameToString(WerewolveServer::Connection c);
    void sendGameInfo(WerewolveServer::Connection);
}