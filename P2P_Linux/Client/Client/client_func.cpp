#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <cinttypes>
#include "client_func.h"
#include "filesyst_func.h"
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <thread>

typedef boost::asio::ip::tcp tcp;

boost::smatch res;
boost::regex regFileName("FileName: *(.+?)\r\n");
boost::regex regFileSize("FileSize: *([0-9]+?)\r\n");

TalkToServer::TalkToServer(const std::string& ip, const int& port_, boost::asio::io_service& io):ip_address(ip),sock(io),acc_sock(io),listener(io),sender(io),acceptor_(io,tcp::endpoint(tcp::v4(), (8090))),port(port_),acc(io,tcp::endpoint(tcp::v4(), (port_))){}


TalkToServer::~TalkToServer()
{
}

bool TalkToServer::connect()
{
    tcp::endpoint ep(boost::asio::ip::address::from_string(ip_address), port);
    sock.connect(ep,ec);
    if(ec)
    {
        return 0;
    }
    return 1;
}


void TalkToServer::sendFileList(const std::vector<std::string>& files,std::vector<std::string>& all_files,const std::string& currIP)
{
    boost::asio::streambuf sb;
    std::ostream os(&sb);
    os<<"Update"<<"\n";
    os<<currIP<<"\n";
    for(size_t i=0;i<files.size();i++)
    {
        os<<files[i]<<"\n";
    }
    boost::asio::write(sock,sb,ec);
    if(ec)
    {
        std::cerr << "connection failed\n";
    }
    sock.shutdown(tcp::socket::shutdown_send);
    sock.close();

    acc.accept(acc_sock,ec);
    if(ec)
    {
        std::cerr << "accept failed\n";
    }
    boost::asio::read_until(acc_sock,sb,"\0",ec);
    if(ec)
    {
        std::cerr << "read_until failed" << ec << std::endl;
    }
    std::istream is(&sb);
    char bfr[512];
    all_files.clear();
    while(is)
    {
        is.getline(bfr,512);
        if(strcmp(bfr,""))
            all_files.push_back(bfr);
    }

    acc_sock.shutdown(tcp::socket::shutdown_both);
    acc_sock.close();
    acc.close();
}


void TalkToServer::acceptFile()
{
    std::cout<<"start to accepting file\n";
    boost::asio::streambuf sBuf;
    std::istream istr(&sBuf);
    acc.accept(acc_sock,ec);
    std::size_t l = boost::asio::read_until(acc_sock, sBuf, boost::regex("(?:\r\n){2,}"), ec);
    if(ec){
        std::cerr << "read_until failed\nl = " << l <<"\n";
    }
    std::string headers, tmp;
    while(std::getline(istr, tmp) && tmp != "\r")
    {
            headers += (tmp + '\n');
    }
    if(!boost::regex_search(headers, res, regFileSize))
    {
        std::cerr << "regFileSize not found\n";
        std::cin.get();
    }
    std::size_t fileSize = boost::lexical_cast<std::size_t>(res[1]);
    if(!boost::regex_search(headers, res, regFileName))
    {
        std::cerr << "regFileName not found\n";
    }
    std::string fileName = res[1];
    std::ofstream ofs(("Files/" + fileName).c_str(), std::ios::binary);
    if(!ofs)
    {
        std::cerr << "Can't create file\n";
        std::cin.get();
    }
    std::size_t len = 0;
    if(sBuf.size())
    {
        len += sBuf.size();
        ofs << &sBuf;
    }
    std::cout<<"all ok in here. Start reading\n";
    while(len != fileSize)
    {
        len += boost::asio::read(acc_sock, sBuf, boost::asio::transfer_at_least(1));
        ofs << &sBuf;
        std::cout << "len = " << len << std::endl;
    }
    ofs.close();
    istr.rdbuf(NULL);
    //sock.shutdown(tcp::socket::shutdown_both);
    sock.close();
    acc.close();
}


void listenLoop(boost::asio::io_service& ioser)
{
    while(true)
    {
        boost::system::error_code erc;
        tcp::socket listener(ioser);
        tcp::socket sender(ioser);
        boost::asio::streambuf sb;
        std::istream is(&sb);
        tcp::acceptor acceptor_(ioser,tcp::endpoint(tcp::v4(),8090));
        std::cout<<"i am in loop, ready to listen in 8090 port\n";
        acceptor_.accept(listener,erc);
        if(erc)
        {
            std::cerr << "accept in 8090 failed failed\n";
        }
        boost::asio::read_until(listener,sb,"\0",erc);
        if(erc)
        {
            std::cerr << "read_until in 8090 failed" << erc << std::endl;
        }
        char bfr[512];
        std::vector<std::string> commands;
        while(is)
        {
            is.getline(bfr,512);
            if(strcmp(bfr,""))
                commands.push_back(bfr);
        }

        for(size_t i = 0; i<commands.size();i++)
            std::cout<<commands[i]<<"\n";

        listener.shutdown(tcp::socket::shutdown_both);
        listener.close();
        acceptor_.close();
        tcp::endpoint ep(boost::asio::ip::address::from_string(commands[1]), 9090);
        sender.connect(ep,erc);
        std::cout<<"try to connect to "<<commands[1]<<" in 9090 port\n";
        if(erc)
        {
            std::cerr << "connection error in 9090 port" << erc << std::endl;
        }
        std::vector<std::string> splitFile;
        split(splitFile,commands[0]);
        std::string filePath("Files/" + splitFile[0]);
        std::cout<<"filepath = "<<filePath<<"\n";
        std::ifstream ifs(filePath.c_str(), std::ios::binary);
        if(!ifs)
        {
            std::cerr << "Can't read file\n";
        }
        std::uintmax_t fileSize = boost::filesystem::file_size(boost::filesystem::path(filePath));
        boost::asio::write(sender, boost::asio::buffer("FileName: " + boost::filesystem::path(filePath).filename().string() + "\r\n"));
        boost::asio::write(sender, boost::asio::buffer("FileSize: " + boost::lexical_cast<std::string>(fileSize) + "\r\n\r\n"));
        std::uintmax_t sentFileBody = 0;
        char fileBuf[10000];
        std::size_t fileBufSize = sizeof(fileBuf);
        while(ifs){
            ifs.read(fileBuf, fileBufSize);
            sentFileBody += boost::asio::write(sender, boost::asio::buffer(fileBuf, ifs.gcount()));
        }
        if(sentFileBody != fileSize)
        {
            std::cerr << "write failed\n";
            std::cin.get();
        }
        ifs.close();
        sender.shutdown(tcp::socket::shutdown_both);
        sender.close();
        std::cout<<"loop ended\n";
    }

}





