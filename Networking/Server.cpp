#pragma once
#include "tcp_connection.hpp"
#include "./../turn.cpp"

#define JOIN 1
#define WEREWOLVEVOTING 2
#define WITCH 3
#define SEER 4
#define WEREWOLVEKILL 5
#define VOTING 6
#define EXECUTION 7
#define KILLED 1
#define HEALED 2

class Server
{
public:
    Server();
    ~Server();
    void start();
    void handle_accept(error_code_t ec, std::shared_ptr<connection_t> con);
    void write_greeting(std::shared_ptr<connection_t> con);
    void listen_for_answer(std::shared_ptr<connection_t> con);
    void handle_client_answer(std::shared_ptr<connection_t> con);
    void handle_write(error_code_t ec, size_t);
    void phase_refreshing();

private:
    Game* hostData;
    char phase;
    char phaseCounter;
    char playerCount;
    char idCounter = 1;
    std::vector<std::shared_ptr<connection_t>> _connections;
    boost::asio::io_service _io_service;
    tcp::acceptor _acceptor;
    tcp::endpoint _endpoint{tcp::v4(), 8888};
    std::unique_ptr<std::thread> _thread;
};

Server::Server() : _io_service{}, _acceptor(this->_io_service)
{
}

Server::~Server()
{
}

void Server::start()
{
    _acceptor.open(_endpoint.protocol());
    _acceptor.bind(_endpoint);
    _acceptor.listen();

    auto connection = std::make_shared<connection_t>(_io_service);
    _acceptor.async_accept(connection->_sock, [this, connection](error_code_t ec) { this->handle_accept(ec, connection); });
    _thread = std::make_unique<std::thread>([this]() { _io_service.run(); });
    _thread->join();
}

void Server::handle_accept(error_code_t ec, std::shared_ptr<connection_t> con)
{
    if (!ec)
    {
        write_phase(con);
        // ready for new connections
        auto new_connection = std::make_shared<connection_t>(_io_service);
        _acceptor.async_accept(new_connection->_sock, [this, new_connection](error_code_t ec) { this->handle_accept(ec, new_connection); });
    }
}

void Server::handle_client_answer(std::shared_ptr<connection_t> con)
{
    std::string client_answer = con->buf;
    char* client_answer_cstring = client_answer.c_str();
    std::cout << "Got the following answer" << client_answer << std::endl;
    std::string server_answer = "" ;
    switch(phase)
    {   
        case JOIN:
        char id = client_answer_cstring[0];
        std::string name = client_answer.substr(1,10);
        server_answer = "Joined the game as " + name; 
        break;
        case WEREWOLVEVOTING:
        char victimID = client_answer_cstring[0];
        //increase votingCount for vitimID
        break;
        case WITCH:
        switch(client_answer_cstring[0])
        {
            case KILLED:
            char victimID = client_answer_cstring[1];
            //kill the vitim
            break;
            case HEALED:
            //skip WWKILL
            break;
        }
        break;
        case SEER:
        
        break;
        case WEREWOLVEKILL:

        break;
        case VOTING:

        break;
        case EXECUTION:

        break;
    }

    strcpy(con->buf, server_answer.c_str());

    auto write_handler = [con](error_code_t ec, size_t written) {
        if (!ec)
        {
            std::cout << "Fertig!" << std::endl;
            listen_for_answer(con);
        }
    };
    auto buf = boost::asio::buffer(con->buf, 100);
    boost::asio::async_write(con->_sock, buf, write_handler);
}

void Server::listen_for_answer(std::shared_ptr<connection_t> con)
{
    auto read_handler = [this, con](error_code_t ec, size_t read) {if(!ec){this->handle_client_answer(con);}else{std::cout << ec.message() << std::endl;} };
    auto buf = boost::asio::buffer(con->buf, 100);
    boost::asio::async_read(con->_sock, buf, read_handler);
    std::cout << "Listening for answer" << std::endl;
}

void Server::write_phase(std::shared_ptr<connection> con)
{
    auto write_handler = [this, con](error_code_t ec, size_t written) {
        if (!ec)
        {
            std::cout << "Message of " << written << "bytes written!" << std::endl;
            listen_for_answer(con);
        }
    };
    std::string message = "";
    switch(phase)
    {
        case JOIN:
        message="1";
        message += idCounter++;
        break;
        case WEREWOLVEVOTING:
        message="2";
        phaseCounter++;
        break;
        case WITCH:
        message="3";
        phaseCounter++;
        break;
        case SEER:
        message="4";
        phaseCounter++;
        break;
        case WEREWOLVEKILL:
        message="5";
        phaseCounter++;
        break;
        case VOTING:
        message="6;";
        phaseCounter++;
        break;
        case EXECUTION:
        message="7";
        phaseCounter++;
        break;
    }
    strcpy(con->buf, message);
    std::cout << con->buf << std::endl;
    auto buf = boost::asio::buffer(con->buf, 100);
    boost::asio::async_write(con->_sock, buf, write_handler);
}

void Server::phase_refreshing(){
    switch(phase){
        case JOIN:
        if(idCounter==playerCount)
        {
            phaseCounter=0;
            phase = WEREWOLVEVOTING;
        }
        break;
        case WEREWOLVEVOTING:
        if(phaseCounter==hostData->werewolveCount)
        {
            phaseCounter=0;
            phase = WITCH;
        }
        break;
        case  WITCH:
        if(phaseCounter==1)
        {
            phaseCounter=0;
            phase = WEREWOLVEKILL;
        }
        break;
        case WEREWOLVEKILL:
        if(phaseCounter==hostData->alivePlayers.size())
        {
            phaseCounter=0;
            phase = SEER;
        }
        break;
        case SEER:
        if(phaseCounter==1)
        {
            phaseCounter=0;
            phase = VOTING;
        }
        break;
        case VOTING:
        if(phaseCounter==hostData->alivePlayers.size())
        {
            phaseCounter=0;
            phase = EXECUTION;
        }   
        break;
        case EXECUTION:
        if(phaseCounter==hostData->alivePlayers.size())
        {
            phaseCounter=0;
            phase = WEREWOLVEVOTING;
        }
    }
}

int main()
{
    Server serv;
    serv.start();
}