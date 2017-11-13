#include"stdafx.h"
#include "ListPath.h"


namespace Client
{
	std::list<std::pair<int,std::string>> ListPath::File_path_list(const std::string& nameWhat){
		InfoDirectory(nameWhat);
		return std::move(oblist);
	}

	void ListPath::InfoDirectory(const std::string & nameD){
		std::string Path;
		std::string str = nameD + "\\*";;
	 
		_finddata_t info = { 0 };
		auto handle = _findfirst(str.c_str(), &info);
		if (handle == MYERROR){
			std::cout << "//throw ErrorOpeningfolderWhenReadingInformationException()" << std::endl;
			exit(1);
		}

		do
		{
			if (std::string(info.name) == "." || std::string(info.name) == "..")
				continue;

			if (info.attrib & _A_SYSTEM)
				continue;

			if (info.attrib & _A_SUBDIR){ 	// folder
				Path = nameD + "\\" + info.name;
				InfoDirectory(Path);
			}
			else
				this->oblist.push_back(std::make_pair(info.size,info.name));
			
		} while (!_findnext(handle, &info));
		_findclose(handle);
	}

}