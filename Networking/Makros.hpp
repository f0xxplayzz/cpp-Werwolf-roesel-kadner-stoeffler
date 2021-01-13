#pragma once

#define STD_WRITE_HANDLER \
        auto write_handler = [this,con](error_code_t ec,size_t written)\
        {\
            if(!ec)\
            {\
                this->listen_for_answer(con);\
            }\
        };

#define STD_ASYNC_WRITE(msg) \
        strcpy(con->buf,msg);\
        auto buf =boost::asio::buffer(con->buf,BUFFERLENGTH);\
        boost::asio::async_write(con->_sock,buf,write_handler);

#define STD_READ_HANDLER\
        auto read_handler = [this,con](error_code_t ec , size_t read)\
        {\
            if(!ec)\
            {\
                this->handle_answer(con);\
            }else\
            {\
                std::cout << ec.message() << std::endl;\
            }\
        };

#define STD_ASYNC_READ \
        auto buf = boost::asio::buffer(con->buf,BUFFERLENGTH);\
        boost::asio::async_read(con->_sock,buf,read_handler);

#define INPUT_BEFORE_CLOSE\
        std::string temp;\
        std::cin >> temp;

#define STD_HEADER char* res = new char[16];createStdHeader(res,id,roleST);