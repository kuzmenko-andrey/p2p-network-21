#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

typedef boost::asio::ip::tcp tcp;

#ifndef SERVER_FUNCT_H
#define SERVER_FUNCT_H

class Server
{
private:
    tcp::socket sock;
    tcp::acceptor acc;
    boost::system::error_code ec;
    int port;
public:
    Server(const int&,boost::asio::io_service&);
    ~Server();
    void listen();
    void acceptFile();
    void chooseCommand(std::map<std::string,std::string>&);
    void sendFileList(const std::vector<std::string>& files);
};

void split(std::vector<std::string>&, std::string);

#endif // SERVER_FUNCT_H
