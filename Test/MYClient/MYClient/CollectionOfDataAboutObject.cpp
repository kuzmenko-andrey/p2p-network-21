#include"stdafx.h"
#include "CollectionOfDataAboutObject.h"


	CollectionOfDataAboutObject::CollectionOfDataAboutObject(const std::string & name ,bool delet)
		:namaWhat(name),sizeFolder(0),delet(delet)
	{}


	std::list<std::string> CollectionOfDataAboutObject::File_path_list()
	{
			bool all = false; 

			InfoDirectory(this->delet, this->namaWhat,all);

			return std::move(this->oblist);
	}


	void CollectionOfDataAboutObject::InfoDirectory(bool deLete, 
		                         const std::string & nameD,
		                            bool all)
	{
		std::string Path;
		std::string str = nameD;
		if (all)
			str += "\\*";
		else
			str += "\\*.*";
	
		_finddata_t info = { 0 };
		auto handle = _findfirst(str.c_str(), &info);
		if (handle == ERROR) 
		{
			std::cout << "//throw ErrorOpeningfolderWhenReadingInformationException()" << std::endl;
			exit(1);
		}

		do
		{
			if (std::string(info.name) == "." || std::string(info.name) == "..") 
				continue;

			if (info.attrib & _A_SYSTEM)
				continue;

			if (info.attrib & _A_SUBDIR) // folder
			{
				Path = nameD + "\\" + info.name;
			
					//this->sizeFolder += info.size;
					
					PushObject(deLete, Path, info.size, true);
		
				InfoDirectory(deLete, Path,all);
			}
			else
			{
				Path = nameD + "\\" + info.name;
		
				this->sizeFolder += info.size;
				PushObject(deLete, Path, info.size, false);
			}
		} while (!_findnext(handle, &info));
		_findclose(handle);
	}

	void CollectionOfDataAboutObject::PushObject(bool & deLete, std::string & Path,
		                                         _fsize_t & size, bool folder)
	{
		if (deLete)
			this->oblist.push_back(Path);
		else
			this->oblist.push_front(Path);
	}
