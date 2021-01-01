#pragma once
#include <boost/asio.hpp>
#include  "./../turn.cpp"

using tcp=boost::asio::ip::tcp;

namespace WerewolveServer{
class Connection{
    public:
    tcp::socket m_sock;
    char *buf;
    Connection(boost::asio::io_service &io_service,int i) : m_sock(io_service), buf(new char[200]){}
    ~Connection()
    {
        delete[] buf;
        m_sock.close();
    }
};
class Server{
    public:
    Game* hostData;
    std::vector<werewolveClient::Client> clients;
    void broadcast(std::string);
    void host(Game* hostData);
};
}