#pragma once
#include "./../turn.cpp"
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include "Server.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

void host(){

    Game hostData;
    boost::asio::io_service my_service;

    tcp::endpoint endpoint{tcp::v4(),8999};

    tcp::acceptor my_acceptor{my_service};
    my_acceptor.open(endpoint.protocol());
    my_acceptor.bind(endpoint);
    my_acceptor.listen();

    auto connection =std::make_shared<WerewolveServer::Connection>(my_service);

    my_acceptor.async_accept(connection->m_sock,[connection,&my_acceptor,&hostData](const error_code_t &ec)
    {
        if(!ec)
        {
            auto boostBuffer = boost::asio::buffer(connection->buf,200);

            boost::asio::async_read(connection->m_sock,boostBuffer,
                [connection,&hostData](error_code_t &ec)
                {
                    char* bufferAsString = connection->buf;
                    switch(bufferAsString[0]){
                        case 0:
                            hostData = processPlayerInfo(connection);
                        case 1:
                            //sendGameInfo(connection);
                            break;
                    }
                });
        }
        my_acceptor.listen();
    });

    auto func =[&my_service](){my_service.run();};
    std::thread t(func);
    t.join();
}

Game processPlayerInfo(std::shared_ptr<WerewolveServer::Connection> c){
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
std::vector<Player> processAlivePlayers(char* c){
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
        tempPlayer->role="";
        tempPlayer->alive=true;
        tempPlayer->voteCounter=c[iterator++];
        alives.push_back(*tempPlayer);
    }while(c[++iterator] != '\0');
    return alives;
}
std::vector<Villager> processVillagers(char* c){
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
std::vector<Werewolve> processWerewolves(char* c){
    int iterator = 0;
    std::vector<Werewolve> werewolves;
    do{
        Werewolve * tempPlayer= new Werewolve;
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
