
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
};


void Client::start()
{   
        std::cout << "Starting Client" << std::endl;
        boost::asio::io_service io_service;

        auto connection = std::make_shared<connection_t>(io_service);

        char buf[100];
        boost::asio::ip::tcp::resolver resolver{io_service};
        boost::asio::ip::tcp::resolver::query query(tcp::v4(),"127.0.0.1", "8999");
        auto it = resolver.resolve(query);

        auto read_handler2 = [this,connection](error_code_t e, size_t r) {
            std::cout << "Answer:" << connection->buf << std::endl;
        };

        auto write_handler = [this,connection, read_handler2](error_code_t e, size_t r) {
            if (!e)
            {
                std::cout << "Answer: " << connection->buf << std::endl;
                std::cout << "Wait for the reaction!" << std::endl;
                auto buf = boost::asio::buffer(connection->buf, 100);
                boost::asio::async_read(connection->_sock, buf, read_handler2);
            }
        };

        auto read_handler = [this,connection, write_handler](error_code_t e, size_t r) {
            //überarbeiten
            if (!e)
            {
                std::cout << "Content: " << connection->buf << std::endl;
                std::cout << "Enter your answer: ";
                std::string answer;
                std::getline(std::cin, answer);
                char cbuf[100];
                strcpy(cbuf,answer.c_str());
                auto buf = boost::asio::buffer(cbuf, 100);
                boost::asio::async_write(connection->_sock, buf, write_handler);
            }
        };

        /*
        auto conn_handler = [this,read_handler, connection](error_code_t ec,auto x) {
            if (!ec)
            {

                auto bbuf = boost::asio::buffer(connection->buf, 100);
                boost::asio::async_read(connection->_sock, bbuf, read_handler);
            }
        };
        */

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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
    auto buf =boost::asio::buffer(con->buf,100);
    boost::asio::async_write(con->_sock,buf,write_handler);
}

void Client::handle_server_answer(std::shared_ptr<connection_t> con)
{
    std::string server_answer = con->buf;
    char* server_answer_cString = to_cString(server_answer);
    char* client_answer = new char[16];
    client_answer[0]=id;
    client_answer[1]=role;
    if(phase=server_answer_cString[0])
    {
        switch(phase)
        {
            case WEREWOLVEVOTING:
            {
                if(role==2)
                {
                    std::vector<char> ids;
                    for(int i = 1;i <= server_answer_cString[1];i++)
                    {
                        ids.push_back(server_answer_cString[1+i]);
                    }
                    int start = ids.size()+3;
                    std::string output = server_answer.substr(start,server_answer.length()-start);
                    std::cout << "The following people can be killed:" << std::endl;
                    std::cout << output << std::endl;
                    int vote = 0;
                    char result;
                    do
                    {
                        std::cin >> vote;
                    } while (vote>0 && vote<ids.size());
                    char voted = ids[vote];
                    client_answer[2] = phase;
                    phase = phase + 2;
                    client_answer[3] = DONE;
                    client_answer[5] = voted;
                }
                else
                { 
                    phase = phase + 2;
                    requestAfterSleep(con);
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
                if(role== SEER)
                {
                    std::vector<char> roles;
                    int lengthV = (int) server_answer_cString[1];
                    for(int i = 1;i <= lengthV ;i++)
                        {
                            roles.push_back(server_answer_cString[1+i]);
                        }
                    int start = roles.size()+3;
                    std::string output = server_answer.substr(start,server_answer.length()-start);
                    std::cout << "Which persons role do you want to see:" << std::endl;
                    std::cout << output << std::endl;
                    int vote = 0;
                    do
                    {
                        std::cin >> vote;
                    } while (vote>0 && vote<roles.size());
                    char result = roles[vote];
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
                    phase++;
                    requestAfterSleep(con);
                }
            }
            break;
            case WEREWOLVEKILL:
            {
                std::string result = server_answer.substr(4,10);
                phase++;
                std::cout << "The following person died tonight: " << result << std::endl;
            }
            break;
            case VOTING:
            {
                std::vector<char> ids;
                for(int i = 1;i <= server_answer_cString[1];i++)
                {
                    ids.push_back(server_answer_cString[1+i]);
                }
                int start = ids.size()+3;
                std::string output = server_answer.substr(start,server_answer.length()-start);
                std::cout << "The following people can be voted to be executed:" << std::endl;
                std::cout << output << std::endl;
                int vote = 0;
                char result;
                do
                {
                    std::cin >> vote;
                } while (vote>0 && vote<ids.size());
                char voted = ids[vote];
                client_answer[2] = phase++;
                client_answer[3] = DONE;
                client_answer[5] = voted;
            }
            break;
            case EXECUTION:
            {
                std::string result = server_answer.substr(4,10);
                phase = WEREWOLVEVOTING;
                std::cout << "The following person was executed: " << result << std::endl;
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
        auto buf =boost::asio::buffer(con->buf,100);
        boost::asio::async_write(con->_sock,buf,write_handler);
    }
    else if (server_answer_cString[0]=9)
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
    auto buf = boost::asio::buffer(con->buf,100);
    boost::asio::async_read(con->_sock,buf,read_handler);
    std::cout << "Listening for answer" << std::endl;
}

void Client::handle_accept(error_code_t ec,std::shared_ptr<connection_t> con)
{
    receive_join(con);
}

void Client::receive_join(std::shared_ptr<connection_t> con)
{
    std::string message = con->buf;
    char* msg_c = to_cString(message);
    id = msg_c[1];
    role = msg_c[2];
    std::string name;
    do
    {
        std::cout << "Enter your name: ";
        std::cin >> name;
    } while (name.length()>10);
    char* name_c = to_cString(name);
    auto write_handler_join = [this,con](error_code_t ec,size_t written)
        {
            if(!ec)
            {
                this->listen_for_answer(con);
            }
        };
    char* join_msg = new char[16];
    join_msg[0]=id;
    join_msg[1]=role;
    join_msg[2]=JOIN;
    int rem;
    for(int i=0;i< name.length()-1;i++)
    {
        join_msg[i+6]=name_c[i];
        rem=i;
    }
    if(rem<10)
    {
        join_msg[rem]=' ';
        rem++;
    }
    
    strcpy(con->buf,join_msg);
    auto buf =boost::asio::buffer(con->buf,100);
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

}