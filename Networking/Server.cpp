#include "tcp_connection.hpp"
#include "./../turn.cpp"
#include "Utilities.hpp"


#define JOIN 1
#define WEREWOLVEVOTING 2
#define WITCH 3
#define SEER 4
#define WEREWOLVEKILL 5
#define VOTING 6
#define EXECUTION 7
#define KILLED 1
#define HEALED 2
#define DO_NOTHING 3
#define NOTDONE 1
#define DONE 2
#define SKIPPED 3

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
    tcp::endpoint _endpoint{tcp::v4(), 8999};
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

void Server::handle_client_answer(std::shared_ptr<connection_t> con)
{
    std::string client_answer = con->buf;
    char* client_answer_cstring = to_cString(client_answer);
    std::string server_answer = "" ;
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
                //createPlayer method verwenden
                std::shared_ptr<Player> p = std::make_shared<Player>();
                p->alive=true;
                p->role=client_answer_cstring[1];
                p->id=id;
                p->name = name;
                hostData->alivePlayers->push_back(p);
                switch(client_answer_cstring[1])
                {
                    case 1:
                    hostData->villagers->push_back(p);
                    case 2:
                    hostData->werewolves->push_back(p);
                    break;
                    case 3:
                    hostData->seers->push_back(p);
                    break;
                    case 4:
                    hostData->witches->push_back(p);
                    break;
                }
                server_answer = "";
                server_answer += phase;
                server_answer += "Joined the game as ";
                server_answer += name;
                join_messages += name;
                join_messages += " joined the game" ;
                join_messages += '\n' ;
                server_answer += '\n' ;
                server_answer += join_messages ;
                std::cout << name << " joined the game"<<std::endl;
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
                    int iterator = 1;
			        for (int i = 0; i < hostData->alivePlayers->size();i++) 
                    {
                        if(hostData->alivePlayers->at(i)->role != 2)
                        {
                            result += std::to_string(iterator++); 
                            result += ".) ";
                            result += hostData->alivePlayers->at(i)->name;
                            result += '\n';
                            id_vector.push_back(hostData->alivePlayers->at(i)->id);
                        }
			        }
                    server_answer= "";
                    server_answer += phase;
                    server_answer += (char) id_vector.size();
                    for(int i=0;i<id_vector.size();i++)
                    {
                        server_answer += id_vector.at(i);
                    }
                    server_answer += result;
                }
                break;
                case DONE:
                /*
                    Increases votingCount for the player who was voted by a werewolve
                    Persons will be killed in the step WEREWOLVEKILL separetly
                */
                {
                    std::cout <<"Received vote" << std::endl;
                    for(std::shared_ptr<Player> p : *hostData->alivePlayers.get())
                        if(p->id==client_answer_cstring[5])
                        {
                            p->voteCounter++;
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
                        int iterator = 1;
                        for (int i = 0; i < hostData->alivePlayers->size(); i++) 
                        {
				            if (hostData->alivePlayers->at(i)->role != 3 ) 
                            {
                                result += std::to_string(iterator++);
                                result += "). ";
                                result += hostData->alivePlayers->at(i)->name;
                                result += '\n';
					            role_vector.push_back(hostData->alivePlayers->at(i)->role);
				            }
                        }
                        server_answer= "";
                        server_answer += phase;
                        server_answer += (char) role_vector.size();
                        for(int i=0;i<role_vector.size();i++)
                        {
                            server_answer += role_vector.at(i);
                        }
                        server_answer += result;
                    }
                    break;
                    case DONE:
                    {
                        std::cout << "Seer has chosen" << std::endl;
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
                        char killedPlayerID = hostData->getMostVoted();
                        server_answer = "";
                        server_answer += phase;
                        server_answer += 1;
                        server_answer += killedPlayerID;
                        std::string name = hostData->getMostVoted_name();
                        server_answer += name;
                        std::cout << server_answer << std::endl;
                    }
                    break;
                    case DONE:
                    {
                        phaseCounter++;
                        server_answer += phase;
                        if(phaseCounter>=hostData->alivePlayers->size())
                        {
                            phase=VOTING;
                            std::cout << "Changed Phase to VOTING" <<std::endl;
                            phaseCounter=0;
                            hostData->executeVotes();
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
                         int iterator = 1;
                        for (int i = 0; i < hostData->alivePlayers->size();i++) 
                        {
                            if(hostData->alivePlayers->at(i)->id != client_answer_cstring[0])
                            {
                                result += std::to_string(iterator++); 
                                result += ".) ";
                                result += hostData->alivePlayers->at(i)->name;
                                result += '\n';
                                id_vector.push_back(hostData->alivePlayers->at(i)->id);
                            }
                        }
                        server_answer = "";
                        server_answer += VOTING;
                        server_answer += (char) id_vector.size();
                        for(int i=0;i<id_vector.size();i++)
                        {
                            server_answer += id_vector.at(i);
                        }
                        server_answer += result;
                    }
                    break;
                    case DONE:
                    {
                        for(std::shared_ptr<Player> p : *hostData->alivePlayers.get())
                            if(p->id=client_answer_cstring[5])
                                p->voteCounter++;
                        phaseCounter++;
                        if(phaseCounter>=hostData->alivePlayers->size())
                        {
                            phase=EXECUTION;
                            std::cout << "Changed phase to EXECUTION" <<std::endl;
                            phaseCounter=0;
                        }
                        server_answer +=phase;
                    }
                    break;
                }
            }
            break;
            case EXECUTION:
            {
                phaseCounter++;
                int killedPlayerID = hostData->getMostVoted();
                std::string killedPlayerName = "";
                    server_answer += phase;
                    server_answer += 1;
                    server_answer += killedPlayerID;
                    for(std::shared_ptr<Player> p : *hostData->alivePlayers)
                    {
                        if(killedPlayerID == p->id)
                        {
                            killedPlayerName = p->name;
                        }
                    }
                    server_answer += killedPlayerName;
                if(phaseCounter>=hostData->alivePlayers->size())
                {
                    phase=WEREWOLVEVOTING;
                    std::cout << "Changed phase to WEREWOLVEVOTING" << std::endl;
                    phaseCounter=0;
                    hostData->executeVotes();
                }
            }
            break;
        }
        strcpy(con->buf, server_answer.c_str());
        auto write_handler = [this,con](error_code_t ec, size_t written) {
            if (!ec)
            {
                listen_for_answer(con);
            }
        };
        auto buf = boost::asio::buffer(con->buf, BUFFERLENGTH);
        boost::asio::async_write(con->_sock, buf, write_handler);
    }
    else
    {
        char* sleeping_msg = new char[1];
        sleeping_msg[0]=phase;
        auto write_handler = [this,con](error_code_t ec, size_t written) 
        {
            if (!ec)
            {   
                listen_for_answer(con);
            }
        };
        strcpy(con->buf,sleeping_msg);
        auto buf = boost::asio::buffer(con->buf, BUFFERLENGTH);
        boost::asio::async_write(con->_sock, buf, write_handler);
    }
}

void Server::listen_for_answer(std::shared_ptr<connection_t> con)
{
    auto read_handler = [this, con](error_code_t ec, size_t read)
    {
        if(!ec)
        {
            this->handle_client_answer(con);
        }else
        {
            std::cout << ec.message() << std::endl;
        } 
    };
    auto buf = boost::asio::buffer(con->buf, BUFFERLENGTH);
    boost::asio::async_read(con->_sock, buf, read_handler);
}

void Server::write_join(std::shared_ptr<connection> con)
{
    auto write_handler = [this, con](error_code_t ec, size_t written) {
        if (!ec)
        {
            std::cout << "Message of " << written << "bytes written!" << std::endl;
            listen_for_answer(con);
        }
    };
    char* msg = new char [BUFFERLENGTH];
    msg[0] = idCounter++;
    std::cout << "Client connected with ID: "<< (int) msg[0] <<std::endl;
    msg[1] = roles.at(roles.size()-1);
    roles.pop_back();
    strcpy(con->buf,msg);
    std::cout << con->buf << std::endl;
    auto joinbuf = boost::asio::buffer(con->buf, BUFFERLENGTH);
    boost::asio::async_write(con->_sock, joinbuf, write_handler);
}


int main()
{
    Server serv;
    serv.start();
}