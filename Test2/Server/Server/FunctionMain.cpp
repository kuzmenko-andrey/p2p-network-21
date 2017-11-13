#include "stdafx.h"
#include "FunctionMain.h"

namespace Example {

	typedef void(*ptr_funcrion)(const std::map<std::string, int>&, std::string&, std::string&);

	struct _MENU_ {
		int ind;
		ptr_funcrion ptr;
	};

	_MENU_ functions[] = {
		{ FunctionMain::ALlFiles,      FunctionMain::AllPath},
		{ FunctionMain::ENdingF,       FunctionMain::AllFilesWithAnEnd },
		{ FunctionMain::FIndFirst,     FunctionMain::FindFirstSimbol },
		{ FunctionMain::FIndFrom_A_to, FunctionMain::FindFrom_A_to_Z},
		{ FunctionMain::FIndStarsONE , FunctionMain::FindStarsONE },
		{ FunctionMain::FIndSomthing,  FunctionMain::FindSomething}
	};

	std::string FunctionMain::GetPaths(const std::map<std::string, int>& maPaths, std::string& strMode)
	{
		std::string paths;
		int rez =GetMode(strMode);
		if(rez > ZERO )
		functions[rez - ONE].ptr(maPaths, strMode, paths);

		return (paths.size() > ZERO) ? paths : " File not found /";
	}

	int FunctionMain::GetMode(std::string& strMode) {
		int rez = -1;
		int find = 0;
		if (strMode.size() == TWO) {
			if (strMode[ZERO] == '*')
				rez = ALlFiles;
		}

		find = strMode.find(".");

		if (rez < ZERO) {
			if (find == ONE && (strMode.size() == SIX))
				rez = ENdingF; // *.txt

			if (strMode.size() == THREE && (strMode[ONE] == '*'))
				rez = FIndFirst; // a*

			if (strMode.size() == FOUR && (strMode[ONE] == '*'))
				rez = FIndFrom_A_to; // a*a

			if (strMode.size() == FOUR && (strMode[ZERO] == '*') && (strMode[ONE] == '.'))
				rez = FIndStarsONE;
		}
		if (rez < ZERO && (find > TWO)) {
			rez = FIndSomthing;
		}
		return rez;
	}

	void FunctionMain::AllPath(const std::map<std::string, int>& maPaths, std::string& strMode, std::string& allpath) {
		for (const auto iter : maPaths)
			Packaging(allpath, iter.first);
	}

	void FunctionMain::Packaging(std::string& allpath, const std::string& what) {
		allpath.append(what);
		allpath.append("/");
	}

	void FunctionMain::AllFilesWithAnEnd(
		const std::map<std::string, int>& maPaths,
		std::string& strMode,
		std::string& allpath)
	{
		strMode = strMode.substr(ONE, strMode.length()); // .jpg = *.jpg
		for (const auto iter : maPaths) {
			std::string buf = iter.first;
			buf = buf.substr(buf.find('.'), buf.length());
			if (buf.compare(strMode) == ZERO) {
				Packaging(allpath, iter.first);
			}
		}
	}

	void FunctionMain::FindFirstSimbol(             // a*
		const std::map<std::string, int>& maPaths,
		std::string& strMode,
		std::string& allpath)
	{
		for (const auto iter : maPaths) {
			if (iter.first[ZERO] == strMode[ZERO])
				Packaging(allpath, iter.first);
		}
	}

	void FunctionMain::FindFrom_A_to_Z(            // a*b
		const std::map<std::string, int>& maPaths,
		std::string& strMode,
		std::string& allpath)
	{
		for (const auto iter : maPaths) {
			if (iter.first[ZERO] == strMode[ZERO]) {
				if (iter.first[iter.first.find('.') - ONE] == strMode[TWO])
					Packaging(allpath, iter.first);
			}
		}
	}

	void FunctionMain::FindStarsONE(
		const std::map<std::string, int>& maPaths,
		std::string& strMode,
		std::string& allpath)
	{
		for (const auto iter : maPaths) {
				if (iter.first[iter.first.find('.') + ONE] == strMode[TWO])
					Packaging(allpath, iter.first);
		}
	}

	void FunctionMain::FindSomething(
		const std::map<std::string, int>& maPaths,
		std::string& strMode,
		std::string& allpath)
	{
		for (const auto iter : maPaths) {
			if (iter.first == strMode)
				Packaging(allpath, iter.first);
		}
	}
}



