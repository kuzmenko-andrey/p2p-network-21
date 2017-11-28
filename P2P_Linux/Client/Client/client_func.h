#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <cinttypes>

#ifndef CLIENT_FUNC_H
#define CLIENT_FUNC_H

typedef boost::asio::ip::tcp tcp;

class TalkToServer
{
   private:
    int port;
    std::string ip_address;
    tcp::socket sock;
    tcp::socket acc_sock;
    tcp::acceptor acc;
    boost::system::error_code ec;

    tcp::socket listener;
    tcp::socket sender;
    tcp::acceptor acceptor_ ;

   public:

    TalkToServer(const std::string&,const int&,boost::asio::io_service&);
    ~TalkToServer();
    bool connect();
    void sendFileList(const std::vector<std::string>&,std::vector<std::string>&,const std::string&);
    void acceptFile();
};

void listenLoop(boost::asio::io_service&);

#endif // CLIENT_FUNC_H
