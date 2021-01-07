#pragma once
//#include "./../turn.cpp"
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
//#include "Server.hpp"
//#include "Networking.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

 /*void WerewolveServer::Server::host(Game* hostData){

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
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(idCounter++,200),[connection](error_code_t err){});
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
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer(phase,200),[connection](error_code_t err){});
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
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer("",200),[connection](error_code_t err){});
                            break;
                        case 6:
                            //werewolveVoting
                            for(Player p: hostData->alivePlayers){
                                if(p.id==bufferAsString[1])
                                    p.voteCounter++;
                            }
                            boost::asio::async_write(connection->m_sock,boost::asio::buffer("",200),[connection](error_code_t err){});
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
*/
typedef struct connection
{
    tcp::socket m_sock;
    char *buf;
    connection(boost::asio::io_service &io_service) : m_sock(io_service), buf(new char[100])
    {
    }
    ~connection()
    {
        delete[] buf;
        m_sock.close();
    }

} connection_t;

int main()
{
    // io_service ist die wichtigste Komponente
    // sie regelt alles rund um das asynchrone
    // eventhandling. --> Erstmal Blackbox
    boost::asio::io_service my_io_service;
    // das ip Protokoll sowie der Port
    tcp::endpoint endpoint{tcp::v4(), 8999};
    // Komponente zum akzeptieren von Verbindungen
    tcp::acceptor acceptor{my_io_service};
    acceptor.open(endpoint.protocol());
    // das festlegen auf den Port, ist dieser Port bereits
    // belegt, gibt es eine exception
    acceptor.bind(endpoint);
    // lausche auf dem Port nach neuen Verbindungen
    acceptor.listen();

    // connection als shared pointer, da sie in async lambdas
    // als Kopie(!) mitgegeben wird
    auto con = std::make_shared<connection>(my_io_service);
    acceptor.async_accept(con->m_sock, [con,&acceptor](const error_code_t &ec) {
        if (!ec)
        {
            // buffer ist der Boost Way um einen block mit fester
            // größe zum lesen zu erzeugen. Benötigt einen Speicher-
            // bereich und die gewünschte Menge an bytes, die gelesen
            // werden sollen
            auto boost_buf = boost::asio::buffer(con->buf, 100);
            // liest so lange asynchron von diesem Socket bis
            // der buffer voll ist, oder die Verbindung abgebrochen wird
            // wir geben die Connection immer mit, damit wir wissen, dass
            // das Object zu 100% gültig ist, wenn die Funktion ausgeführt wird
            boost::asio::async_read(con->m_sock, boost_buf, 
            [con](error_code_t ec, size_t len) {
                std::cout << con->buf;
                std::reverse(con->buf, con->buf + 100);
                auto buf = boost::asio::buffer(con->buf, 100);
                // async_write ruft die handler Funktion auf, sobald es den
                // kompletten buffer in das Socket geschrieben hat
                boost::asio::async_write(con->m_sock, buf,
                    [con](error_code_t ec, size_t len) { 
                        std::cout << std::endl << "i did my job" << std::endl; });
            });
            //boost::asio::async_read();
        }
        acceptor.async_accept();
    });
    // Erstellen eines threads in dem der io_service gestartet wird
    // Der Thread endet erst wenn der Event-Loop leer ist,
    // also keine asynchrone Aktion mehr ausgeführt wird
    // das geschieht bei einem Server in der Regel erst wenn er
    // beendet wird
    auto thread_func = [&my_io_service]() { my_io_service.run(); };
    std::thread t(thread_func);
    // Der Server soll solange laufen bis der Event-Loop leer ist
    t.join();
}