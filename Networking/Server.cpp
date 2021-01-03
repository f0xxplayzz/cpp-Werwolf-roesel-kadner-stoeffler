#pragma once
#include "./../turn.cpp"
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include "Server.hpp"
#include "Networking.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

 void WerewolveServer::Server::host(Game* hostData){

    boost::asio::io_service my_service;

    tcp::endpoint endpoint{tcp::v4(),8999};

    tcp::acceptor my_acceptor{my_service};
    my_acceptor.open(endpoint.protocol());
    my_acceptor.bind(endpoint);
    my_acceptor.listen();

    auto connection =std::make_shared<WerewolveServer::Connection>(my_service);

    my_acceptor.async_accept(connection->m_sock,[connection,&my_acceptor,&hostData,this](const error_code_t &ec)
    {
        if(!ec)
        {
            auto boostBuffer = boost::asio::buffer(connection->buf,200);

            boost::asio::async_read(connection->m_sock,boostBuffer,
                [connection,&hostData,this](error_code_t &ec)
                {
                    char* bufferAsString = connection->buf;
                    std::string bufferString = bufferAsString;
                    switch(bufferAsString[0]){
                        case 1:
                            //join
                            Player *p = new Player();
                            p->alive =true;
                            p->id = bufferAsString[1];
                            p->role = bufferAsString[2];
                            p->name = bufferString.substr(3,10);
                            hostData->alivePlayers.push_back(*p);
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(hostData->toString() + idCounter++,200),[connection](error_code_t err){});
                            break;
                        case 2:
                            //actionOfClient
                            for(Player p : hostData->alivePlayers){
                                if(p.id==bufferAsString[1]){
                                    switch(bufferAsString[3]){
                                        case 1:
                                            //Villager
                                            break;
                                        case 2:
                                            //Werewolve
                                            for(Player target : hostData->alivePlayers){
                                                if(target.id == bufferAsString[2])
                                                    target.voteCounter++;
                                            }
                                            break;
                                    }
                                }
                            }
                            break;
                        case 3:
                            //requestData
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(hostData->toString(),200),[connection](error_code_t err){});
                            break;
                        case 4:
                            //ChatMessage
                            //not implemented yet
                            break;
                        case 5:
                            //voting
                            for(Player p: hostData->alivePlayers){
                                if(p.id==bufferAsString[1])
                                    p.voteCounter++;
                            }
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(hostData->toString(),200),[connection](error_code_t err){});
                            break;
                        case 6:
                            //werewolveVoting
                            for(Player p: hostData->alivePlayers){
                                if(p.id==bufferAsString[1])
                                    p.voteCounter++;
                            }
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(hostData->toString(),200),[connection](error_code_t err){});
                            break;
                    }
                });
            my_acceptor.listen();
        }
        my_acceptor.accept();
    });

    auto func =[&my_service](){my_service.run();};
    std::thread t(func);
    t.join();
}