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
        
        void setSocket();
        void openConnection(char cases);
        std::string createJoinMessage();
        std::string createActionMessage(char id);
        std::string createChatMessage(std::string msg);
        std::string createVotingMessage(char id);
        std::string createWerewolveVotingMessage(char id);

        char getRole();
        std::string getName();
        bool getLivingStatus();
    };
}