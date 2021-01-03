#pragma once
#include <boost/asio.hpp>
#include "./../turn.cpp"


using boost::asio::ip::tcp;
namespace werewolveClient{
    class Client{
        char id; //ID of the player-object
        Game clientData;
        boost::asio::io_service my_service;
        tcp::socket socket;
        boost::system::error_code err;
        
        //Client.cpp
        void setSocket();
        void openConnection(char cases);
        void join();
        void action(char id, char ca);
        void chatMessage(std::string msg);
        void voting(char id);
        void werewolveVoting(char id);
        void openConnection();
        void closeConnection();
        Game processPlayerInfoOnJoin(std::string);

        //ClientMessages.cpp
        std::string createJoinMessage();
        std::string createActionMessage(char id, char ca);
        std::string createChatMessage(std::string msg);
        std::string createVotingMessage(char id);
        std::string createWerewolveVotingMessage(char id);

        //ClientUtilities.cpp
        char getRole();
        std::string getName();
        bool getLivingStatus();
    };
}