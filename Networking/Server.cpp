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
    //void handle_write(error_code_t ec, size_t);
    std::vector<char> roles;

private:
    Game* hostData;
    char phase;
    char phaseCounter;
    char playerCount=4;
    char idCounter = 1;
    std::vector<std::shared_ptr<connection_t>> _connections;
    boost::asio::io_service _io_service;
    tcp::acceptor _acceptor;
    tcp::endpoint _endpoint{tcp::v4(), 2233};
    std::unique_ptr<std::thread> _thread;
};

Server::Server() : _io_service{}, _acceptor(this->_io_service)
{
    //testing Data
    roles.push_back(1);
    roles.push_back(1);
    roles.push_back(1);
    roles.push_back(2);
    //testing Data
    phase = JOIN;
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
    std::string result;
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
                Player* p = new Player();
                p->alive=true;
                p->role=client_answer_cstring[1];
                p->id=id;
                //std::shared_ptr<Player> player_ptr = std::make_shared<Player>(p);
                //hostData->alivePlayers->push_back(player_ptr);
                /*switch(p->role)
                {
                    case 2:
                    hostData->werewolves->push_back(p);
                    break;
                    case 3:
                    hostData->seers->push_back(p);
                    break;
                    case 4:
                    hostData->witches->push_back(p);
                    break;
                }*/
                server_answer += phase;
                server_answer += "Joined the game as "; 
                server_answer += name;
                std::cout << name << " joined the game"<<std::endl;
                phaseCounter++;
                if(phaseCounter>=playerCount)
                {
                    phase=WEREWOLVEVOTING;
                    std::cout << "Changed Phase" <<std::endl;
                    phaseCounter=0;
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
			        for (int i = 0; i < hostData->villagers->size();i++) 
                    {
                        result += iterator++; 
                        result += ".) ";
                        result += hostData->villagers->at(i)->name;
                        result += '\n';
                        id_vector.push_back(hostData->villagers->at(i)->id);
			        }
                }
                break;
                case DONE:
                /*
                    Increases votingCount for the player who was voted by a werewolve
                    Persons will be killed in the step WEREWOLVEKILL separetly
                */
                {
                    for(std::shared_ptr<Player> p : *hostData->alivePlayers.get())
                        if(p->id=client_answer_cstring[5])
                            p->voteCounter++;
                    phaseCounter++;
                    if(phaseCounter>=hostData->werewolves->size())
                    {
                        phase = SEER;
                        phaseCounter = 0;
                    }
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
				            if (client_answer_cstring[0] != hostData->alivePlayers->at(i)->id) 
                            {
                                result += iterator++;
                                result += "). ";
                                result += hostData->alivePlayers->at(i)->name;
                                result += '\n';
					            role_vector.push_back(hostData->alivePlayers->at(i)->role);
				            }
                        }
                    }
                    break;
                    case DONE:
                    {
                        phase++;
                    }
                    break;
                }

            }
            break;
            case WEREWOLVEKILL:
                phaseCounter++;
                if(phaseCounter>=hostData->alivePlayers->size())
                {
                    phase++;
                    phaseCounter=0;
                }
                hostData->executeVotes();
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
                            result += iterator++; 
                            result += ".) ";
                            result += hostData->villagers->at(i)->name;
                            result += '\n';
                            id_vector.push_back(hostData->alivePlayers->at(i)->id);
                        }
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
                                phase++;
                                phaseCounter=0;
                            }
                    }
                    break;
                }
            }
            break;
            case EXECUTION:
                phase = WEREWOLVEVOTING;
                phaseCounter++;
                if(phaseCounter>=hostData->alivePlayers->size())
                {
                    phase++;
                    phaseCounter=0;
                }
                hostData->executeVotes();
            break;
        }
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