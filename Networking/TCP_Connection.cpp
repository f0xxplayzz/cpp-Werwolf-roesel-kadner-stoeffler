#pragma once 
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

class tcp_connection
{
    int id;
    tcp::socket m_sock;
    char *buf;
    tcp_connection(boost::asio::io_service &io_service) : m_sock(io_service), buf(new char[100])
    {
    }
    ~tcp_connection()
    {
        delete[] buf;
        m_sock.close();
    }

};