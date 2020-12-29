#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;
typedef struct connection
{
    tcp::socket m_sock;
    char *buf;
    connection(boost::asio::io_service &io_service) : m_sock(io_service), buf(new char[10])
    {
    }
    ~connection()
    {
        delete[] buf;
        m_sock.close();
    }
} connection_t;

int main()
{
    boost::asio::io_service my_io_service;
    tcp::endpoint endpoint{tcp::v4(), 8999};
    tcp::acceptor acceptor{my_io_service};
    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();

    auto con = std::make_shared<connection>(my_io_service);
    acceptor.async_accept(con->m_sock, [con](const error_code_t &ec) {
        if (!ec)
        {
            auto boost_buf = boost::asio::buffer(con->buf, 10);
            boost::asio::async_read(con->m_sock, boost_buf,
                                    [con](error_code_t ec, size_t len) {
                                        std::cout << con->buf;
                                        std::reverse(con->buf, con->buf + 10);
                                        auto buf = boost::asio::buffer(con->buf, 10);

                                        boost::asio::async_write(con->m_sock, buf, [con](error_code_t ec, size_t len) { std::cout << std::endl
                                                                                                                                  << "i did my job" << std::endl; });
                                    });
        }
    });

    auto thread_func = [&my_io_service]() { my_io_service.run(); };
    std::thread t(thread_func);
    t.join();
}