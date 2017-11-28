#include "server_funct.h"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <bitset>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <boost/algorithm/string.hpp>

typedef boost::asio::ip::tcp tcp;

Server::Server(const int& port_,boost::asio::io_service& io):port(port_),sock(io),acc(io,tcp::endpoint(tcp::v4(), port_)){}
Server::~Server(){}

void split(std::vector<std::string>& strs, std::string str) //делит строку по разделителю ":"
{
    boost::split(strs, str, boost::is_any_of(":"));
}


void Server::listen() //слушает входящие соединения
{
    acc.accept(sock);
    std::cout << "Client has been accepted\n";
}

void Server::chooseCommand(std::map<std::string,std::string>& file_map) //выполняет действия в зависимости от входной команды
{
    std::vector<std::string> commands;
    boost::asio::streambuf sb;
    boost::asio::read_until(sock,sb,"\n");
    std::istream is(&sb);
    char bfr[256];
    while(is)
    {
        is.getline(bfr,256);
        commands.push_back(bfr);
    }
    for(size_t i = 0;i<commands.size();i++)
        std::cout<<commands[i]<<"\n";
    sock.shutdown(tcp::socket::shutdown_both);
    sock.close();
    acc.close();

    if(commands[0]=="Update") //обновляет и отправляет клиенту список файлов
    {
        std::cout<<"Updating\n";
        if(file_map.empty())
        {
            for(size_t i=2;i<commands.size();i++)
            {
                if(commands[i]!="")
                {
                    file_map.insert(std::pair<std::string,std::string>(commands[i],commands[1]));
                }
            }
        }
        else
        {
            for(size_t i = 2; i < commands.size();i++)
            {
                if(commands[i]!="")
                {
                    std::map<std::string,std::string>::iterator iter=file_map.find(commands[i]);
                    if(iter==file_map.end())
                        file_map.insert(std::pair<std::string,std::string>(commands[i],commands[1]));
                    else
                    {
                      if(!(strstr(  ((*iter).second).c_str(),commands[1].c_str())))
                        (*iter).second = (*iter).second + ":" + commands[1];
                    }
                }
            }

        }

        for(std::map<std::string,std::string>::iterator it = file_map.begin();it!=file_map.end();it++)
        {
            std::cout<<(*it).first<<" : "<<(*it).second<<"\n";
        }

        tcp::endpoint endp(boost::asio::ip::address::from_string(commands[1]),9090);
        sock.connect(endp);
        std::ostream os(&sb);

        bool gflag = false;
        for(std::map<std::string,std::string>::iterator mit = file_map.begin();mit!=file_map.end();mit++)
        {

            bool flag = true;
            for(size_t i = 2; i!=commands.size(); i++)
            {
                if((*mit).first==commands[i])
                {
                    flag=false;
                    break;
                }

            }
            if(flag)
            {
                os<<(*mit).first<<"\n";
                gflag=true;
            }
        }

        if(!gflag)
            os<<":NoNewFiles\n";
        os<<"\0";
        std::cout<<sb.size()<<" ";
        size_t bts=boost::asio::write(sock,sb,ec);
        if(ec){std::cerr << "read_until failed\n;";}
        std::cout<<bts<<"\n";
        std::cout<<"sended\n";
        sock.shutdown(tcp::socket::shutdown_both);
        sock.close();
    }
    else if(commands[0]=="Send") //отправляет клиенту запрос на скачивание файла
    {
        for(size_t i=0;i<commands.size();i++)
            std::cout<<commands[i]<<"\n";
        std::ostream os(&sb);
        os<<commands[1]<<"\n";
        os<<commands[2]<<"\n";
        os<<"\0";
        std::map<std::string,std::string>::iterator iter = file_map.find(commands[1]);
        std::vector<std::string> owners;
        for(std::map<std::string,std::string>::iterator itr=file_map.begin();itr!=file_map.end();itr++)
            std::cout<<(*itr).first<<" : "<<(*itr).second<<"\n";
        if(iter!=file_map.end())
        {
            if(strstr((*iter).second.c_str(),":"))
            {
                std::cout<<"a lot of owners";
                split(owners,(*iter).second);
            }
            else
            {
                owners.push_back((*iter).second);
                std::cout<<"single owner: "<<owners[0];
            }
            bool flag=false;
                std::cout<<"\n";
            for(size_t i=0;i<owners.size();i++)
                std::cout<<"owner "<<i+1<<": "<<owners[i]<<"\n";
            for(size_t i = 0; i<owners.size();i++)
            {
                tcp::endpoint endp(boost::asio::ip::address::from_string(owners[i]),8090);
                sock.connect(endp,ec);
                if(ec)
                {
                   continue;
                }
                std::cout<<"connected with client`s loop\n";
                flag=true;
                boost::asio::write(sock,sb,ec);
                if(!flag)
                {
                    //send acceptor that all clients dont answer
                }
            }
        }
        else
        {
            //send acceptor that file now found

        }
        //sock.shutdown(tcp::socket::shutdown_send);
        sock.close();
    }
    else if(commands[0]=="Disconnect") //удаляет из списка файлы отключившегося клиента
    {
            std::string userIP=commands[1];
            bool flag=true;
            while(flag)
            {
                flag=false;
                for(std::map<std::string,std::string>::iterator mit = file_map.begin();mit!=file_map.end();mit++)
                {
                    if(strstr((*mit).second.c_str(),userIP.c_str()))
                    {
                      if(strstr((*mit).second.c_str(),":")) //for a lot of owners
                      {
                         std::string before = ":" + commands[1];
                         std::string after = commands[1] + ":";
                         if(strstr((*mit).second.c_str(),before.c_str())) //for :127.0.0.1
                         {
                             size_t pos=(*mit).second.find(commands[1]);
                             if(pos)
                                 (*mit).second.erase(pos,commands[1].size()+1);
                             std::cout<<"iteration\n";
                         }
                         else if(strstr((*mit).second.c_str(),after.c_str())) //for 127.0.0.1:
                         {
                             size_t pos=(*mit).second.find(commands[1]);
                             if(pos)
                                 (*mit).second.erase(pos,commands[1].size()+1);
                             std::cout<<"iteration\n";
                         }
                      }
                      else //for single owner
                      {
                          flag=true;
                          file_map.erase((*mit).first);
                          std::cout<<"iteration\n";
                          break;
                      }
                    }
                }

        }
    }
}



