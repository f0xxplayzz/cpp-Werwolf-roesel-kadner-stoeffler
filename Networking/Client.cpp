
#pragma once
#include "tcp_connection.h"
#include "Utilities.hpp"
#include <stdio.h>
#include <iostream>
#include "Definitions.h"
#include "Makros.h"
#include "Networkmessaging/ClientMessages.hpp"
#include "./../narrator.cpp"

class Client
{
    public:
    void start();
    void listen_for_answer(std::shared_ptr<connection_t> con);
    void receive_join(std::shared_ptr<connection_t> con);
    void handle_answer(std::shared_ptr<connection_t> con);
    void handle_write(std::shared_ptr<connection_t> con);
    void handle_accept(error_code_t ec,std::shared_ptr<connection_t> con);
    void goSleeping();
    void requestAfterSleep(std::shared_ptr<connection_t> con);

    Client()
    {
        narrator = new Narrator();
    }
    private:
    Narrator* narrator;
    char phase;
    char id;
    char role;
    char name;
    bool gameOver = false;
    bool joined =false;
};


void Client::start()
{   
        /*
            Starts the client and opens a connection to the Server
        */
        boost::asio::io_service io_service;
        auto connection = std::make_shared<connection_t>(io_service);
        char buf[BUFFERLENGTH];
        //Opening a resolver
        boost::asio::ip::tcp::resolver resolver{io_service};
        //resolve Server-Address
        boost::asio::ip::tcp::resolver::query query(tcp::v4(), SERVER_IP, PORT);
        auto it = resolver.resolve(query);
        //Connect to Server
        boost::asio::async_connect(connection->_sock, it,[this,connection] (const boost::system::error_code &ec,auto x)
            {
                this->handle_accept(ec,connection);
            });
        //Multithreading is possible
        auto t = std::thread([&io_service]() { io_service.run(); });
        t.join();
}

void Client::goSleeping()
{
    /*
        Let the Thread which calls go sleeping for a while
        Works on every OS since c++ std11
    */
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEPINGTIME));
}

void Client::requestAfterSleep(std::shared_ptr<connection_t> con)
{
    /*
        Lets the client sleep before he request Data from the Server again
    */
    //Go sleeping first
    goSleeping();
    //create a Write_Handler
    STD_WRITE_HANDLER
    //create the request
    char* message =  ClientMessages::createDataRequest(id,role,phase);
    //send the request
    STD_ASYNC_WRITE(message)
}

void Client::handle_answer(std::shared_ptr<connection_t> con)
{
    /*
        Processes Server-Answer and creates an answer if needed
    */
    //copy message in the scope
    std::string server_answer = con->buf;
    //convert answer to a cstring
    char* server_answer_cString = to_cString(server_answer);
    //create a ptr for the answer
    char* client_answer;
    //if  client and server have the same phase go into the switch-case
    if(phase==server_answer_cString[0])
    {
        switch(phase)
        {
            case WEREWOLVEVOTING:
            {
                /*
                    Let the player vote to kill another player if he is a werewolve
                */
                
                if(role==WEREWOLVE_ROLE)
                {
                    narrator->gameStartWerewolve();
                    narrator->dayEndWerewolve();
                    narrator->werewolfNightStart();
                    //reads Data and processes it so the player can vote
                    std::vector<char> ids;
                    for(int i = 1;i <= server_answer_cString[1];i++)
                    {
                        ids.push_back(server_answer_cString[1+i]);
                    }
                    int start = ids.size()+2;
                    //create a substring to get voting possibilities as a string
                    std::string output = server_answer.substr(start);
                    narrator->voteKillStart();
                    std::cout << output << std::endl;
                    int vote = 0;
                    char result;
                    //Let the Player choose one Player to kill
                    narrator->voteKillDecision();
                    do
                    {
                        std::string temp;
                        std::cin >> temp;
                        vote = std::stoi(temp);
                    } while (!(vote>0) && !(vote<=ids.size()));
                    char voted = ids[vote-1];
                    //create the answer, ready to send
                    client_answer =  ClientMessages::createWerewolveVoting(voted,id,role);
                    phase = SEER;
                    narrator->fallAsleep();
                }
                else
                { 
                    narrator->gameStartVillage();
                    narrator->dayEndVillager();
                    //skips Voting-Event
                    client_answer =  ClientMessages::createSkipMessage(id,role,phase);
                    phase = SEER;
                }
            }
            break;
            case WITCH:
            {
                //not implemented yet
            }
            break;
            case SEER:
            {
                /*
                    Let a player see the role of another player if he is a Seer
                */

                //check if player is a seer
                if(role==SEER_ROLE)
                {
                    narrator->seeerNightStart();
                    std::vector<char> roles;
                    //process data from Server
                    int lengthV = (int) server_answer_cString[1];
                    for(int i = 1;i <= lengthV ;i++)
                        {
                            roles.push_back(server_answer_cString[1+i]);
                        }
                    int start = roles.size()+2;
                    //creates substring to get possibilities as a string
                    std::string output = server_answer.substr(start);
                    narrator->voteSeerStart();
                    std::cout << output << std::endl;
                    int vote = 1;
                    //vote now
                    narrator->voteSeerDecision();
                    do
                    {
                        std::string temp;
                        std::cin >> temp;
                        vote = std::stoi(temp);
                    } while (!(vote>0 && vote<=roles.size()));
                    int result = roles.at(vote-1);
                    narrator->voteSeerResult(roleAsString(result));
                    //create answer
                    client_answer =  ClientMessages::createSeerCompleted(id,role);
                    phase= WEREWOLVEKILL;
                }
                else
                { 
                    //Skip Event if player is no seer
                    client_answer =  ClientMessages::createSkipMessage(id,role,phase);
                    phase = WEREWOLVEKILL;
                }
            }
            break;
            case WEREWOLVEKILL:
            {
                /*
                    Execute the kill from the Wrewolves now
                */
                std::cout << server_answer << std::endl;
                //if data is already accessible continue, else request Data
                if(!(server_answer.length() <=3))
                {
                    phase=VOTING;
                    std::string result = server_answer.substr(3);
                    if(server_answer_cString[2]==id)
                    {
                        //If Player died, tells him and closes the Client
                        std::cout << "YOU DIED" << std::endl;
                        gameOver = true;
                    }
                    else{
                        //Tell Player who died
                        narrator->deathsKilled(result);
                    }
                    //create answer so server knows how many people checked in
                    client_answer =  ClientMessages::createWerewolveKillCompleted(id,role);
                }
                //requestData if it is not accessible yet
                else requestAfterSleep(con);
            }
            break;
            case VOTING:
            {
                /*
                    Let the players execute another Player
                */
                std::vector<char> ids;
                //process Data
                std::cout<<" "<<std::endl;
                for(int i = 1;i <= server_answer_cString[1];i++)
                {
                    ids.push_back(server_answer_cString[1+i]);
                }
                int start = ids.size()+2;
                //substr to see possibilities as a string
                std::string output = server_answer.substr(start);
                narrator->voteExecutionPossibilities();
                std::cout << output << std::endl;
                int vote = 1;
                //vote now !!
                narrator->voteExecutionChoice();
                do
                {
                    std::string temp;
                    std::cin >> temp;
                    vote = std::stoi(temp);
                } while (!(vote>0 && vote<=ids.size()));
                int voted = ids.at(vote-1);
                phase = EXECUTION;
                //creates answer, tells server who was voted
                client_answer =  ClientMessages::createVoting(voted,id,role);
            }
            break;
            case EXECUTION:
            {
                /*
                    Executes the most voted player
                */
                //if data is accessible continue, else request Data again
                if(!(server_answer.length() <=3))
                {
                    //get name of executed person
                    std::string result = server_answer.substr(3);
                    if(server_answer_cString[2]==id)
                    {
                        //tell player if he died and close the Client
                        std::cout << "YOU DIED" << std::endl;
                        gameOver = true;
                    }
                    else
                    {
                        //tell player who died
                        phase=WEREWOLVEVOTING;
                        narrator->deathsHung(result);
                    }
                    //create answer so the Server knows who has checked in
                    client_answer =  ClientMessages::createExecutionCompleted(id,role);
                }
                else requestAfterSleep(con);
            }
            break;
            case GAMEOVER:
            {
                /*
                    Tell player which winCondition was achieved
                */
                gameOver = true;
                switch(server_answer_cString[1])
                {
                    case 1:
                    std::cout << "The Werewolves have won!" << std::endl;    
                    break;
                    case 2:
                    std::cout << "The Villagers have won!" << std::endl;
                    break;
                    case 3:
                    std::cout << "The Narrator has won!" << std::endl;
                    break;
                    phase=10;
                }
            }
            break;
        }
        if(!gameOver && phase != GAMEOVER)
        {
            STD_WRITE_HANDLER
            STD_ASYNC_WRITE(client_answer)
        }
        else if(gameOver || phase==GAMEOVER)
        {
            auto write_handler = [this,con](error_code_t ec, size_t written)
            {   
                if(!ec)
                {
                    phase = GAMEOVER;
                    listen_for_answer(con);
                }
            };
            client_answer = new char[16];
            ClientMessages::createStdHeader(client_answer,role,id);
            client_answer[2]=phase;
            client_answer[3]= NOTDONE;
            STD_ASYNC_WRITE(client_answer)
        }
    }
    else if (server_answer_cString[0]== JOIN && joined == false)
    {
        std::string out = server_answer.substr(1);
        std::cout << out << std::endl;
        //set flag, so client cant login a second time
        joined=true;
        requestAfterSleep(con);
    }
    else
    {
        if(server_answer_cString[0]==GAMEOVER)
        {
            phase=GAMEOVER;
        }
        requestAfterSleep(con);
    }
     
}

void Client::listen_for_answer(std::shared_ptr<connection_t> con)
{
    /*
        Listens for answers
    */
    STD_READ_HANDLER
    STD_ASYNC_READ
}

void Client::handle_accept(error_code_t ec,std::shared_ptr<connection_t> con)
{
    auto bbuf = boost::asio::buffer (con->buf,BUFFERLENGTH);
    //special read_handler for joining phase
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
    /*
        get all important Data from Server
        send important Data back
    */
    std::string message = con->buf;
    char* msg_c = to_cString(message);
    id = msg_c[0];
    role = msg_c[1];
    std::cout << "Your role is: " << roleAsString(role) <<std::endl;
    std::string name;
    //Get your own name
    do
    {
        std::cout << "Enter your name: "<<std::endl;;
        std::cin >> name;
    } while (name.length()>10);
    phase=WEREWOLVEVOTING;
    STD_WRITE_HANDLER
    char* join_msg = ClientMessages::createJoinMessage(name,id,role);
    STD_ASYNC_WRITE(join_msg)
}
//main will be moved soon
