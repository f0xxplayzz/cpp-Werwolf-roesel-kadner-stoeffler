#pragma once
#include <boost/asio.hpp>
#include  "./../turn.cpp"

using tcp=boost::asio::ip::tcp;

namespace WerewolveServer{
class Connection{
    public:
    int id;
    tcp::socket m_sock;
    char *buf;
    Game hostData;
    Connection(boost::asio::io_service &io_service,int i) : m_sock(io_service), buf(new char[200])
    {
        id = i;
    }
    ~Connection()
    {
        delete[] buf;
        m_sock.close();
    }
};
void host();
}