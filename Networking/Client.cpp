#include "Networking.hpp"
#include "Client.hpp"

using boost::asio::ip::tcp;

void werewolveClient::Client::setSocket(){
    socket = tcp::socket(my_service);
}
void werewolveClient::Client::openConnection(char cases){
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::asio::write(socket,boost::asio::buffer(""),err);
    if(!err){
        std::string message;
        switch(cases){
            case 1:
                //join
                message = createJoinMessage();
                break;  
            case 2:
                //action / sendData
                message = createActionMessage(1);//<- Platzhalter
                break;
            case 3:
    	        //requestData
            case 4:
                //ChatMessage
                message = createChatMessage("");
                break;
            case 5:
                //voting
                message = createVotingMessage(1);
                break;
            case 6:
                //WerewolveVoting
                message = createWerewolveVotingMessage(1);
                break;
        }
    }
    else{
        //errorhandling
    }
    boost::asio::streambuf receiver;
    boost::asio::read(socket,receiver,boost::asio::transfer_all(),err);
    if(err && err != boost::asio::error::eof){
        //errorhandling
    }
    else{
        //handling
    }
}
