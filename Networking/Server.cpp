#include "tcp_connection.hpp"
#include "./../turn.cpp"
#include "Utilities.hpp"
#include "Definitions.hpp"
#include "Networkmessaging/ServerMessages.hpp"


class Server
{
public:
    Server();
    ~Server();
    void start();
    void handle_accept(error_code_t ec, std::shared_ptr<connection_t> con);
    void write_join(std::shared_ptr<connection_t> con);
    void listen_for_answer(std::shared_ptr<connection_t> con);
    void handle_client_answer(std::shared_ptr<connection_t> con);
private:
    Game* hostData;
    std::vector<char> roles;
    std::string join_messages;
    char phase;
    char phaseCounter;
    char playerCount=4;
    char idCounter = 1;
    std::vector<std::shared_ptr<connection_t>> _connections;
    boost::asio::io_service _io_service;
    tcp::acceptor _acceptor;
    tcp::endpoint _endpoint{tcp::v4(), std::stoi(PORT)};
    std::unique_ptr<std::thread> _thread;
};

Server::Server() : _io_service{}, _acceptor(this->_io_service)
{
    //testing Data
    roles.push_back(1);
    roles.push_back(2);
    roles.push_back(3);
    roles.push_back(1);
    //testing Data
    phase = JOIN;
    join_messages = "";
    hostData = new Game();
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
        write_join(con);
        // ready for new connections
        auto new_connection = std::make_shared<connection_t>(_io_service);
        _acceptor.async_accept(new_connection->_sock, [this, new_connection](error_code_t ec) { this->handle_accept(ec, new_connection); });
    }
}

void Server::handle_answer(std::shared_ptr<connection_t> con)
{
    std::string client_answer = con->buf;
    char* client_answer_cstring = to_cString(client_answer);
    std::string server_answer = "";
    std::string result = "";
    std::vector<char> id_vector;
    std::vector<char> role_vector;
    if(phase==client_answer_cstring[2])
    {
        switch(phase)
        {   
            case JOIN:
            {
                char id = client_answer_cstring[0];
                std::string name = client_answer.substr(6);
                std::shared_ptr<Player> p = std::make_shared<Player>();
                p->alive=true;
                p->role=client_answer_cstring[1];
                p->id=id;
                p->name = name;
                hostData->alivePlayers->push_back(p);
                switch(client_answer_cstring[1])
                {
                    case VILLAGER_ROLE:
                    hostData->villagers->push_back(p);
                    case WEREWOLVE_ROLE:
                    hostData->werewolves->push_back(p);
                    break;
                    case SEER_ROLE:
                    hostData->seers->push_back(p);
                    break;
                    case WITCH_ROLE:
                    hostData->witches->push_back(p);
                    break;
                }
                server_answer = ServerMessages::createJoin(phase,name);
                if(hostData->alivePlayers->size()>=playerCount)
                {
                    phaseCounter=0;
                    phase=WEREWOLVEVOTING;
                    std::cout << "Changed Phase to WEREWOLVEVOTING" <<std::endl;
                }
            } 
            break;
            case WEREWOLVEVOTING:
            switch(client_answer_cstring[3])
            {
                case NOTDONE:
                {
                    /*
                        Send length of ID as char
                        Send IDs as chars
                        Send Voting possibilities as String (names)

                        Get Information or Implementation from player.cpp
                    */
                    server_answer = ServerMessages::createWerewolveVoting(hostData);
                }
                break;
                case DONE:
                /*
                    Increases votingCount for the player who was voted by a werewolve
                    Persons will be killed in the step WEREWOLVEKILL separetly
                */
                {
                    int voted = (int) client_answer_cstring[5];
                    int lengthV = hostData->alivePlayers->size();
                    for(int i = 0; i < lengthV;i++)
                    {
                        if(hostData->alivePlayers->at(i)->id == voted)
                        {
                            hostData->alivePlayers->at(i)->voteCounter++;
                        }
                    }
                    phaseCounter++;
                    server_answer +=phase;
                    if(phaseCounter>=hostData->werewolves->size())
                    {
                        phase = SEER;
                        std::cout << "Changed Phase to SEER" << std::endl;
                        phaseCounter = 0;
                    }
                }
                break;
                case SKIPPED:
                {
                    phaseCounter++;
                    if(phaseCounter >= hostData->alivePlayers->size())
                    {
                        phase=SEER;
                        std::cout << "Changed Phase to SEER" <<std::endl;
                        phaseCounter=0;
                    }
                    server_answer +=phase;
                }
                break;
            }
            break;
            //witch not implemented yet
            /*case WITCH:
            switch(client_answer_cstring[3])
            {
                case NOTDONE:
                {
                    Send signal ->choose Kill/Heal/do_nothing
                    char werewolvevictim
                    char number of people who can be killed
                    string of people who can be killed
                }
                break;
                //char victimID = client_answer_cstring[1];
                //kill the victim
                case DONE:
                {
                    switch(client_answer_cstring[4])
                    {
                        case HEALED:
                        {
                                Skips Werewolve Kill by setting every voteCounter to 0
                            for(Player p : hostData->alivePlayers)
                                p.voteCounter=0;
                            phase = SEER;
                        }
                        break;
                        case KILLED:
                        {
                            for(Player p : hostData->alivePlayers)
                                if(p.id=client_answer_cstring[5])
                                    //kill player with that ID
                                    int i = 0;
                        }
                        break;
                        case DO_NOTHING:
                        {
                            std::cout << "The witch has chosen to do nothing" << std::endl;
                        }
                        break;
                    }
                }
                break;
            }
            break;
            */
            case SEER:
            {
                switch(client_answer_cstring[3])
                {
                    case NOTDONE:
                    {
                        /*
                            send number of people who can be elected as char
                            send IDs as chars
                            send names as ONE String
                        */
                        server_answer = ServerMessages::createSeer(hostData);
                    }
                    break;
                    case DONE:
                    {
                        phaseCounter++;
                        server_answer +=phase;
                        if(phaseCounter>=hostData->alivePlayers->size())
                        {
                            phase=WEREWOLVEKILL;
                            std::cout << "Changed Phase to WEREWOLVEKILL" <<std::endl;
                            phaseCounter=0;
                        }
                    }
                    break;
                    case SKIPPED:
                    {
                        phaseCounter++;
                        server_answer += phase;
                        if(phaseCounter >= hostData->alivePlayers->size())
                        {
                            phase=WEREWOLVEKILL;
                            std::cout << "Changed Phase to WEREWOLVEKILL" <<std::endl;
                            phaseCounter=0;
                        }
                    }
                break;
                }

            }
            break;
            case WEREWOLVEKILL:
            {
                switch(client_answer_cstring[3])
                {
                    case NOTDONE:
                    {
                        server_answer = ServerMessages::createWerewolveKill(hostData);
                    }
                    break;
                    case DONE:
                    {
                        phaseCounter++;
                        server_answer += phase;
                        if(phaseCounter==hostData->alivePlayers->size())
                        {
                            phase=VOTING;
                            std::cout << "Changed Phase to VOTING" <<std::endl;
                            phaseCounter=0;
                            hostData->executeVotes();
                            hostData->checkWinCondition();
                            if(hostData->gameOver)
                            {
                                phase=GAMEOVER;
                            }
                        }
                    }
                    break;
                }
            }
            break;
            case VOTING:
            {
                switch(client_answer_cstring[3])
                {
                    case NOTDONE:
                    {
                        /*
                            Send number of vote possibilities as char
                            Send Ids as chars
                            Send names as ONE string
                        */
                        server_answer = ServerMessages::createVoting(hostData, client_answer_cstring[0]);
                    }
                    break;
                    case DONE:
                    {
                        for(int i = 0; i < hostData->alivePlayers->size();i++)
                        {
                            if(hostData->alivePlayers->at(i)->id== (int) client_answer_cstring[5])
                            {
                                hostData->alivePlayers->at(i)->voteCounter++;
                            }
                        }
                        phaseCounter++;
                        server_answer += VOTING;
                        if(phaseCounter>=hostData->alivePlayers->size())
                        {
                            phase=EXECUTION;
                            std::cout << "Changed phase to EXECUTION" <<std::endl;
                            phaseCounter=0;
                        }
                    }
                    break;
                }
            }
            break;
            case EXECUTION:
            {
                switch(client_answer_cstring[3])
                {
                    case NOTDONE:
                    {
                        server_answer = ServerMessages::createExecution(hostData);
                    }
                    break;
                    case DONE:
                    {
                        phaseCounter++;
                        server_answer += EXECUTION;
                        if(phaseCounter >= hostData->alivePlayers->size())
                        {
                            phase=WEREWOLVEVOTING;
                            std::cout << "Changed Phase to WEREWOLVEVOTING" <<std::endl;
                            phaseCounter=0;
                            hostData->executeVotes();
                            hostData->checkWinCondition();
                            if(hostData->gameOver)
                            {
                                phase=GAMEOVER;
                            }
                        }
                    }
                    break;
                }
            }
            break;
            case GAMEOVER:
            {
                server_answer = ServerMessages::createGameover(hostData);
            }
            break;
        }
        STD_WRITE_HANDLER
        STD_ASYNC_WRITE(server_answer.c_str())
    }
    else if(phase != GAMEOVER)
    {
        char* sleeping_msg = new char[1];
        sleeping_msg[0]=phase;
        STD_WRITE_HANDLER
        STD_ASYNC_WRITE(sleeping_msg)
    }
    else
    {
        server_answer = ServerMessages::createGameover(hostData);
        auto write_handler = [this,con](error_code_t ec, size_t written) 
        {
            if (!ec)
            {   
                std::cout << "Closed a connection" << std::endl;
            }
        };
        STD_ASYNC_WRITE(server_answer.c_str())
    }
    
}

void Server::listen_for_answer(std::shared_ptr<connection_t> con)
{
    STD_READ_HANDLER
    STD_ASYNC_READ
}

void Server::write_join(std::shared_ptr<connection> con)
{
    STD_WRITE_HANDLER
    char* msg = new char [BUFFERLENGTH];
    msg[0] = idCounter++;
    std::cout << "Client connected with ID: "<< (int) msg[0] <<std::endl;
    msg[1] = roles.at(roles.size()-1);
    roles.pop_back();
    STD_ASYNC_WRITE(msg)
}


int main()
{
    Server serv;
    serv.start();
}