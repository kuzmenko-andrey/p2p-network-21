#pragma once
#include"stdafx.h"
#include<string>
#include<io.h>
#include<list>

	class CollectionOfDataAboutObject
	{
		std::list<std::string> oblist;

		std::string namaWhat;

		int sizeFolder = 0;
	
		bool delet = false;
	
		CollectionOfDataAboutObject(const CollectionOfDataAboutObject&) = delete;
		CollectionOfDataAboutObject(CollectionOfDataAboutObject &&) = delete;
		CollectionOfDataAboutObject & operator=(const CollectionOfDataAboutObject &) = delete;
		CollectionOfDataAboutObject & operator=(CollectionOfDataAboutObject &&) = delete;
		
	public:

		enum MYSize :int {ERROR = -1};

		CollectionOfDataAboutObject(const std::string &,bool);
		
		std::list<std::string> File_path_list();
		
		void InfoDirectory(bool,const std::string &,bool);

		void PushObject(bool & , std::string &, _fsize_t &, bool);

		int GetSizeFolder() throw()
		{
			return this->sizeFolder;
		}
		~CollectionOfDataAboutObject(){};
	};

