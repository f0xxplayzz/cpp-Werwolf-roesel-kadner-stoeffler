
#include "tcp_connection.hpp"
#include "Utilities.hpp"
#include <stdio.h>
#include <iostream>

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

class Client
{
    public:
    void start();
    void listen_for_answer(std::shared_ptr<connection_t> con);
    void receive_join(std::shared_ptr<connection_t> con);
    void handle_server_answer(std::shared_ptr<connection_t> con);
    void handle_write(std::shared_ptr<connection_t> con);
    void handle_accept(error_code_t ec,std::shared_ptr<connection_t> con);
    void goSleeping();
    void requestAfterSleep(std::shared_ptr<connection_t> con);

    private:
    char* createDataRequest();
    

    char phase;
    char id;
    char role;
    char name;
    bool gameOver = false;
    bool joined =false;
};


void Client::start()
{   
        std::cout << "Starting Client" << std::endl;
        boost::asio::io_service io_service;

        auto connection = std::make_shared<connection_t>(io_service);

        char buf[BUFFERLENGTH];
        boost::asio::ip::tcp::resolver resolver{io_service};
        boost::asio::ip::tcp::resolver::query query(tcp::v4(),"127.0.0.1", "8999");
        auto it = resolver.resolve(query);

        boost::asio::async_connect(connection->_sock, it,[this,connection] (const boost::system::error_code &ec,auto x)
            {
                this->handle_accept(ec,connection);
            });

        auto t = std::thread([&io_service]() { io_service.run(); });
        t.join();
        std::cout << "Closing Client" << std::endl;
}

void Client::goSleeping()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void Client::requestAfterSleep(std::shared_ptr<connection_t> con)
{
    goSleeping();
    auto write_handler = [this,con](error_code_t ec,size_t written)
        {
            if(!ec)
            {
                this->listen_for_answer(con);
            }
        };
    char* message = createDataRequest();
    strcpy(con->buf,message);
    auto buf =boost::asio::buffer(con->buf,BUFFERLENGTH);
    boost::asio::async_write(con->_sock,buf,write_handler);
}

void Client::handle_server_answer(std::shared_ptr<connection_t> con)
{
    std::string server_answer = con->buf;
    char* server_answer_cString = to_cString(server_answer);
    char* client_answer = new char[16];
    client_answer[0]=id;
    client_answer[1]=role;
    if(phase==server_answer_cString[0])
    {
        std::cout << "Same phase as Server" << std::endl;
        switch(phase)
        {
            case WEREWOLVEVOTING:
            {
                std::cout << "The complete village falls asleep" << std::endl;
                std::cout << "The Werewolves wake up" << std::endl;
                if(role==2)
                {
                    std::vector<char> ids;
                    for(int i = 1;i <= server_answer_cString[1];i++)
                    {
                        ids.push_back(server_answer_cString[1+i]);
                    }
                    int start = ids.size()+2;
                    std::string output = server_answer.substr(start);
                    std::cout << "The following people can be killed:" << std::endl;
                    std::cout << output << std::endl;
                    int vote = 0;
                    char result;
                    do
                    {
                        std::string temp;
                        std::cin >> temp;
                        vote = std::stoi(temp);
                    } while (!(vote>0) && !(vote<=ids.size()));
                    char voted = ids.at(vote-1);
                    client_answer[2] = phase;
                    phase = SEER;
                    client_answer[3] = DONE;
                    client_answer[5] = voted;
                }
                else
                { 
                    client_answer[2]=phase;
                    client_answer[3]=SKIPPED;
                    phase = SEER;
                }
                break;
            }
            case WITCH:
            {
                //not implemented yet
            }
            break;
            case SEER:
            {
                std::cout << "The Werewolves fall asleep" << std::endl;
                std::cout << "The Seer wakes up" << std::endl;
                if(role==3)
                {
                    std::vector<char> roles;
                    int lengthV = (int) server_answer_cString[1];
                    for(int i = 1;i <= lengthV ;i++)
                        {
                            roles.push_back(server_answer_cString[1+i]);
                        }
                    int start = roles.size()+2;
                    std::string output = server_answer.substr(start);
                    std::cout << "Which persons role do you want to see:" << std::endl;
                    std::cout << output << std::endl;
                    int vote = 0;
                    do
                    {
                        std::string temp;
                        std::cin >> temp;
                        vote = std::stoi(temp);
                    } while (!(vote>0 && vote<=roles.size()));
                    char result = roles.at(vote-1);
                    std::string roleOutput;
                    switch(result)
                    {
                        case 1:
                        roleOutput = "Villager";
                        break;
                        case 2:
                        roleOutput = "Werewolve";
                        break;
                        case 3:
                        roleOutput = "Seer";
                        break;
                        case 4:
                        roleOutput = "Witch";
                    }
                    std::cout << "The person was a " << roleOutput <<std::endl;
                    client_answer[2] = phase++;
                    client_answer[3] = DONE;
                }
                else
                { 
                    client_answer[2]=phase;
                    client_answer[3]=SKIPPED;
                    phase = WEREWOLVEKILL;
                }
            }
            break;
            case WEREWOLVEKILL:
            {
                std::cout << "The seer falls asleep" << std::endl;
                std::string result = server_answer.substr(3);
                phase++;
                std::cout << "The following person died tonight: " << result << std::endl;
            }
            break;
            case VOTING:
            {
                std::cout << "The complete village wakes up" << std::endl;
                std::vector<char> ids;
                int length = (int) server_answer_cString[1];
                for(int i = 1;i <= length;i++)
                {
                    ids.push_back(server_answer_cString[1+i]);
                }
                int start = ids.size()+2;
                std::string output = server_answer.substr(start);
                std::cout << "The following people can be voted to be executed:" << std::endl;
                std::cout << output << std::endl;
                int vote = 0;
                char result;
                do
                {
                    std::string temp;
                    std::cin >> temp;
                    vote = std::stoi(temp);
                } while (!(vote>0 && vote<=ids.size()));
                char voted = ids.at(vote -1);
                client_answer[2] = phase++;
                client_answer[3] = DONE;
                client_answer[5] = voted;
            }
            break;
            case EXECUTION:
            {
                std::string result = server_answer.substr(4);
                phase = WEREWOLVEVOTING;
                std::cout << "The following person was executed: " << result << std::endl;
                std::cout << "The complete village falls asleep" << std::endl;
            }
            break;
        }
        auto write_handler = [this,con](error_code_t ec, size_t written)
        {
            if(!ec)
            {
                this->listen_for_answer(con);
            }
        };
        strcpy(con->buf,client_answer);
        auto buf =boost::asio::buffer(con->buf,BUFFERLENGTH);
        boost::asio::async_write(con->_sock,buf,write_handler);
    }
    else if (server_answer_cString[0]== JOIN && joined == false)
    {
        std::string out = server_answer.substr(1);
        std::cout << out << std::endl;
        joined=true;
        requestAfterSleep(con);
    }
    else if (server_answer_cString[0]==9)
    {
        std::cout << "Closing Client" << std::endl;
    }
    else
    {
        requestAfterSleep(con);
    }
     
}

void Client::listen_for_answer(std::shared_ptr<connection_t> con)
{
    auto read_handler = [this,con](error_code_t ec , size_t read)
        {
            if(!ec)
            {
                this->handle_server_answer(con);
            }else
            {
                std::cout << ec.message() << std::endl;
            }
        };
    auto buf = boost::asio::buffer(con->buf,BUFFERLENGTH);
    boost::asio::async_read(con->_sock,buf,read_handler);
}

void Client::handle_accept(error_code_t ec,std::shared_ptr<connection_t> con)
{
    auto bbuf = boost::asio::buffer (con->buf,BUFFERLENGTH);
    boost::asio::async_read(con->_sock,bbuf,[this,con](error_code_t ec,size_t)
        {
            if(!ec)
            {
                receive_join(con);
            }
        });
}

void Client::receive_join(std::shared_ptr<connection_t> con)
{
    std::string message = con->buf;
    char* msg_c = to_cString(message);
    std::cout << message << std::endl;
    id = msg_c[0];
    std::cout << "Your ID:" << (int) id <<std::endl; 
    role = msg_c[1];
    std::string role_str;
    switch(role)
    {
        case 1:
        role_str="Villager";
        break;
        case 2:
        role_str="Werewolve";
        break;
        case 3:
        role_str="Seer";
        break;
        case 4:
        role_str="Witch";
        break;
    }
    std::cout << "Your role is: " << role_str <<std::endl;
    std::string name;
    do
    {
        std::cout << "Enter your name: "<<std::endl;;
        std::cin >> name;
    } while (name.length()>10);
    auto write_handler_join = [this,con](error_code_t ec,size_t written)
        {
            if(!ec)
            {
                this->listen_for_answer(con);
            }
        };
    char* join_msg = new char[6];
    join_msg[0]=id;
    join_msg[1]=role;
    join_msg[2]=JOIN;
    join_msg[3]='#';
    join_msg[4]='#';
    join_msg[5]='#';
    std::string final_msg = ""; 
    final_msg += join_msg;
    final_msg += name;
    char* final_msg_c = to_cString(final_msg);
    phase=WEREWOLVEVOTING;
    strcpy(con->buf,final_msg_c);
    auto buf =boost::asio::buffer(con->buf,BUFFERLENGTH);
    boost::asio::async_write(con->_sock,buf,write_handler_join);
}

char* Client::createDataRequest()
{
    char* msg = new char[16];
    msg[0]=id;
    msg[1]=role;
    msg[2]=phase;
    msg[3]=NOTDONE;
    return msg;
}

int main()
{
    Client* c = new Client;
    c->start();
}