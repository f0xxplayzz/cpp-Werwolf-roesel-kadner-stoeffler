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
    std::cout << "Got the following answer" << client_answer << std::endl;
    std::string server_answer = "" ;
    if(phase==client_answer_cstring[2])
    {
        switch(phase)
        {   
            case JOIN:
            {
                char id = client_answer_cstring[0];
                std::string name = client_answer.substr(6,10);
                //TODO: create Player here
                server_answer = "Joined the game as " + name;
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
                   
                }
                break;
                case DONE:
                /*
                    Increses votingCount for the player who was voted by a werewolve
                    Persons will be killed in the step WEREWOLVEKILL separetly
                */
                {
                    for(Player p : hostData->alivePlayers)
                        if(p.id=client_answer_cstring[5])
                            p.voteCounter++;
                }
                break;
            }
            break;
            case WITCH:
            switch(client_answer_cstring[3])
            {
                case NOTDONE:
                {
                /*
                    Send signal ->choose Kill/Heal/do_nothing
                    char werewolvevictim
                    char number of people who can be killed
                    string of people who can be killed
                */
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
                            /*
                                Skips Werewolve Kill by setting every voteCounter to 0
                            */
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
                    }
                    break;
                    case DONE:
                    {
                        char temprole;
                        for(Player p : hostData->alivePlayers)
                                if(p.id=client_answer_cstring[5])
                                    temprole= p.role;
                        //Send temprole to the SEER
                    }
                    break;
                }

            }
            break;
            case WEREWOLVEKILL:
            hostData->executeWerewolveKill();
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
                    }
                    break;
                    case DONE:
                    {
                        for(Player p : hostData->alivePlayers)
                                if(p.id=client_answer_cstring[5])
                                    p.voteCounter++;
                    }
                    break;
                }
            }
            break;
            case EXECUTION:
            //execute voteKill
            break;
        }
    }

    strcpy(con->buf, server_answer.c_str());

    auto write_handler = [this,con](error_code_t ec, size_t written) {
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

void Server::write_join(std::shared_ptr<connection> con)
{
    auto write_handler = [this, con](error_code_t ec, size_t written) {
        if (!ec)
        {
            std::cout << "Message of " << written << "bytes written!" << std::endl;
            listen_for_answer(con);
        }
    };
    std::string message = "1";
    message += idCounter++;
    //message += role;
    //message += 
    char* msg = to_cString(message);
    strcpy(con->buf, msg);
    std::cout << con->buf << std::endl;
    auto buf = boost::asio::buffer(con->buf, 100);
    boost::asio::async_write(con->_sock, buf, write_handler);
}


int main()
{
    Server serv;
    serv.start();
}