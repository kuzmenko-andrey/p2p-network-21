#pragma once
#include"stdafx.h"

namespace Example {
    class FunctionMain 
	{
	public:
		enum { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX };
		enum { ALlFiles = 1, ENdingF, FIndFirst, FIndFrom_A_to,FIndStarsONE,FIndSomthing};

		static int GetMode(std::string&);
		static void Packaging(std::string&, const std::string&);
		static std::string GetPaths(const std::map<std::string, int>&, std::string&);
		
		static void AllPath(const std::map<std::string, int>&, std::string&, std::string&); // * найти все файлы
		static void AllFilesWithAnEnd(const std::map<std::string, int>&,std::string&, std::string&); // все файлы с окончанием *.jpg
		static void FindFirstSimbol(const std::map<std::string, int>&, std::string&, std::string&); // a* найти все файлы которые начинаются с символа(a-z)
		static void FindFrom_A_to_Z(const std::map<std::string, int>&, std::string&, std::string&); // a*b
		static void FindStarsONE(const std::map<std::string, int>&, std::string&, std::string&);   // *.h
		static void FindSomething(const std::map<std::string, int>&, std::string&, std::string&); 
	};
}