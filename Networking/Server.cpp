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
                            hostData = &Network::processPlayerInfo(connection);
                        case 1:
                            //sendGameInfo(connection);
                            break;
                        case 2:
                            //voting
                            break;
                        case 3:
                            //werewolveKill
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


