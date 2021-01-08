#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;  

std::string readFromSocket(tcp::socket & sock){
    boost::asio::streambuf buf;
    boost::asio::read_until(sock,buf,"\n");
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}

void sendMessage(tcp::socket &sock, const std::string &message){
    const std::string msg= message + "\n";
    boost::asio::write(socket,boost::asio::buffer(msg));
}

int main(){

}