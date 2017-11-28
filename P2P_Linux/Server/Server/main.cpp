#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "server_funct.h"
#include <map>

std::map<std::string,std::string> file_list;

typedef boost::asio::ip::tcp tcp;
boost::asio::io_service service;

int main()
{
    std::cout<<"Server is running\n";

        try
        {
            while(true)
            {
                Server serv(9090,service);
                serv.listen();
                serv.chooseCommand(file_list);
                std::cout<<"\nloop\n";
            }
        }
        catch(std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << std::endl;
        }
    std::cin.get();
    return 0;

}
