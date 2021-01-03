#pragma once
#include "./../Networking.hpp"

#define ALIVES server->hostData->alivePlayers.size()
#define WEREWOLVES server->hostData->werewolveCount


#define JOINING 1
#define WEREWOLVETURN 2
#define WEREWOLVEKILL 3
#define VOTING 4
#define EXECUTION 5

void serverMain(int join){
    WerewolveServer::Server* server = new WerewolveServer::Server();
    std::thread hostingThread(server->host());
    server->phase = JOINING;
    while(server->phaseCounter < join){
        std::cout << "Warte auf Mitspieler..." << std::endl;
        Sleep(1000);
    }
    while(!server->hostData->gameOver){
        server->phaseCounter = 0;
        server->phase = WEREWOLVETURN;
        while(server->phaseCounter < WEREWOLVES){
            /*
                WerewolveActions
            */
           Sleep(1000);
        }
        server->phaseCounter = 0;
        server->phase = WEREWOLVEKILL;
        server->hostData->executeWerewolveKill();
        while(server->phaseCounter < ALIVES){
            /*
                Warten bis alle eingecheckt haben und die neuesten Daten empfangen haben
            */
           Sleep(1000);
        }
        server->phaseCounter=0;
        server->phase = VOTING;
        while(server->phaseCounter < ALIVES){
            /*
                Wait until everyone voted
            */
           Sleep(1000);
        }
        server->phaseCounter = 0;
        server->phase = EXECUTION;
        //server->hostData.exceution();
        while(server->phaseCounter < ALIVES){
            /*
            Wait until everyone received who was executed
            */
        }
    }
}