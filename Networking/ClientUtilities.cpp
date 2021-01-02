#pragma once
#include "Client.hpp"
#include <string>

char werewolveClient::Client::getRole(){
    for(Player p :clientData.alivePlayers){
        if(p.id =id)
            return p.role;
    }
}

std::string werewolveClient::Client::getName(){
    char* result;
    for(Player p :clientData.alivePlayers){
        if(p.id =id)
            return p.name;
    }
}

bool std::string werewolveClient::Client::getLivingStatus(){
    for(Player p :clientData.alivePlayers){
        if(p.id =id)
            return p.alive;
    }
}