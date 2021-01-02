#include "Networking.hpp"
#include "Client.hpp"
#include "Networking.hpp"
#include <streambuf.hpp>

using boost::asio::ip::tcp;

void werewolveClient::Client::setSocket(){
    socket = tcp::socket(my_service);
}

void werewolveClient::Client::join(){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::async_write(socket,boost::asio::buffer(createJoinMessage(),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Joined Server"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket.receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::action(char id, char ca){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::async_write(socket,boost::asio::buffer(createActionMessage(id,ca),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Sended action informations"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket.receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::chatMessage(std::string msg){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::async_write(socket,boost::asio::buffer(createChatMessage(msg),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Sended Chat Msg"<<std::endl;
        }
    });
    auto receiver = boost::asio::buffer("",200);
    socket.receive(receiver);
    //receiver -> Chat
}

void werewolveClient::Client::voting(char id){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::async_write(socket,boost::asio::buffer(createVotingMessage(id),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Voted"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket.receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::werewolveVoting(char id){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::async_write(socket,boost::asio::buffer(createWerewolveVotingMessage(id),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"voted"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket.receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}