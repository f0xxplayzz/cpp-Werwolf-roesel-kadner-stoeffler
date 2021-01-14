#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <stdio.h>
#include "Definitions.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

typedef struct connection
{
    tcp::socket _sock;
    char *buf;
    connection(boost::asio::io_service &io_service) : _sock(io_service), buf(new char[BUFFERLENGTH])
    {
    }
    ~connection()
    {
        delete[] buf;
        _sock.close();
    }

} connection_t;