
#include <iostream>
#include <clocale>
#include <string>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>
#include "filesyst_func.h"
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>


namespace bfs = boost::filesystem;

bool vector_find(const std::vector<std::string>& vec,const std::string& elem)
{
    bool flag = false;
    for(int i = 0;i<vec.size();i++)
    {
        if(vec[i]==elem)
        {
            flag = true;
            break;
        }
    }
    return flag;
}

void getFileList(std::vector<std::string>& filenames)
{
    filenames.clear();
    std::string filesDir = boost::filesystem::canonical(bfs::current_path()).string() + "/Files";
    for(bfs::recursive_directory_iterator rdib(filesDir), rdie; rdib != rdie; ++rdib)
    {
        std::uintmax_t t = boost::filesystem::file_size(boost::filesystem::path(filesDir + "/" + rdib->path().filename().string()));
        filenames.push_back( (rdib->path().filename().string()) + ":" + boost::lexical_cast<std::string>(t));
    }
}

std::string currentIP()
{
    boost::system::error_code ec;
    std::string name=boost::asio::ip::host_name(ec);
    if(ec)
    {
        std::cerr<<"host_name() failed: \n"<<ec.message();
    }

    boost::asio::io_service io;
    boost::asio::ip::tcp::resolver r(io);
    boost::asio::ip::tcp::resolver::iterator it = r.resolve(boost::asio::ip::tcp::resolver::query(name, ""), ec), itEnd;
    if(ec)
    {
        std::cerr<<"resolve() failed: \n"<<ec.message();
    }
    std::string temp;
    for(; it != itEnd; ++it)
    {
        temp=it->endpoint().address().to_string();
    }
        return temp;
}

void split(std::vector<std::string>& strs, std::string str)
{
    boost::split(strs, str, boost::is_any_of(":"));
}
