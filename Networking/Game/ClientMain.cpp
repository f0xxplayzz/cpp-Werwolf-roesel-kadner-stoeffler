#pragma once
#include "./../Networking.hpp"

#define JOINING 1
#define WEREWOLVETURN 2
#define WEREWOLVEKILL 3
#define VOTING 4
#define EXECUTION 5

void clientMain(){
    //ToDo
    std::thread refresh(refreshing);
    //std::thread chat();
    werewolveClient::Client *client = new werewolveClient::Client();
    client->setSocket();
    client->openConnection();
    client->join();
    for(Player p : client->clientData.alivePlayers){
        if(p.id==client->id){
            client->role = p.role;
        }
    }
    client->getPlayerData();
    while(!client->clientData.gameOver){
        //One Game  Round
        if(client->phase == WEREWOLVETURN && client->role == 2){
            /*
            
            */
           int temp = client->player->voteKill_network();
           client->werewolveVoting(temp);
        }
        if(client->phase == VOTING){
            /*

            */
        }
        Sleep(1000);
    }
    refresh.join();
    client->closeConnection();
}
void refreshing(werewolveClient::Client* c){
    while(!c->clientData.gameOver){
    c->requestData();
    c->getPlayerData();
    Sleep(1000);
    }
}
void chat(){
    //Threadfunction for the chat not implemented yet
}