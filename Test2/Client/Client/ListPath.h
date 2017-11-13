#pragma once
#include"stdafx.h"
#include<string>
#include<io.h>
#include<list>


namespace Client
{
	class ListPath
	{
		std::list<std::pair<int,std::string>> oblist;
		
		ListPath(const ListPath&) = delete;
		ListPath(ListPath &&) = delete;
		ListPath & operator=(const ListPath &) = delete;
		ListPath & operator=(ListPath &&) = delete;
	public:
		enum MYSize :int { MYERROR = -1 };

		ListPath(void) {}

	    std::list<std::pair<int,std::string>> File_path_list(const std::string&);

		void InfoDirectory(const std::string &);

		~ListPath(void) {};
	};

}