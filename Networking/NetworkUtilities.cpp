#pragma once
#include "Networking.hpp"

Game Network::processPlayerInfo(std::shared_ptr<WerewolveServer::Connection> c){
    char* data = c->buf;
    char* alivePlayers;
    char* villagers;
    char* werewolves;
    Game newData;
    newData.werewolveCount=data[1];
    newData.gameOver = data[2];
    int iterator = 3;
    char comp = 'a';
    int k=0;
    while(comp != '#'){
        alivePlayers[k++]=data[iterator++];
        comp=data[iterator];
    }
    comp='a';
    iterator++;
    k=0;
    while(comp != '#'){
        villagers[k++]=data[iterator++];
        comp=data[iterator];
    }
    comp='a';
    iterator++;
    k=0;
    while(comp != '#'){
    	werewolves[k++]=data[iterator++];
        comp=data[iterator];
    }
    newData.alivePlayers=processAlivePlayers(alivePlayers);
    newData.villagers=processVillagers(villagers);
    newData.werewolves=processWerewolves(werewolves);
    return newData;
}
std::vector<Player> Network::processAlivePlayers(char* c){
    int iterator = 0;
    std::vector<Player> alives;
    do{
        Player * tempPlayer= new Player;
        char comp = 'a';
        std::string temp = "";
        while(comp!='?')
        {
            temp += c[iterator++];
            comp = c[iterator];
        }
        tempPlayer->name=temp;
        tempPlayer->role;
        tempPlayer->alive=true;
        tempPlayer->voteCounter=c[iterator++];
        alives.push_back(*tempPlayer);
    }while(c[++iterator] != '\0');
    return alives;
}
std::vector<Villager> Network::processVillagers(char* c){
    int iterator = 0;
    std::vector<Villager> villagers;
    do{
        Villager * tempPlayer= new Villager;
        char comp = 'a';
        std::string temp = "";
        while(comp!='?')
        {
            temp += c[iterator++];
            comp = c[iterator];
        }
        tempPlayer->name=temp;
        tempPlayer->alive=true;
        tempPlayer->voteCounter=c[iterator++];
        villagers.push_back(*tempPlayer);
    }while(c[++iterator] != '\0');
    return villagers;
}
std::vector<Werewolves::Werewolve> Network::processWerewolves(char* c){
    int iterator = 0;
    std::vector<Werewolves::Werewolve> werewolves;
    do{
        Werewolves::Werewolve * tempPlayer= new Werewolves::Werewolve;
        char comp = 'a';
        std::string temp = "";
        while(comp!='?')
        {
            temp += c[iterator++];
            comp = c[iterator];
        }
        tempPlayer->name=temp;
        tempPlayer->role="";
        tempPlayer->alive=true;
        tempPlayer->voteCounter=c[iterator++];
        werewolves.push_back(*tempPlayer);
    }while(c[++iterator] != '\0');
    return werewolves;
}