#include "Networking.hpp"
#include "Client.hpp"

using boost::asio::ip::tcp;

void werewolveClient::openConnection(char cases){
    boost::asio::io_service my_service;
    tcp::socket socket(my_service);
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
    boost::system::error_code err;
    boost::asio::write(socket,boost::asio::buffer(""),err);
    if(!err){
        switch(cases){
            case 1:
                
            case 2:

            case 3:

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
