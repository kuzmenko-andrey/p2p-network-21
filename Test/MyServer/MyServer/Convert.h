#pragma once
#include"stdafx.h"
#include <vector>
#include <string>


namespace Example
{
	class Convert
	{
		enum { FullLength = -1 };
	public:
		static std::vector<unsigned char> DataToVector(const int & iCode,
			const std::string &strText)
		{
			std::vector<unsigned char> vec(sizeof(iCode)+strText.length());
			memcpy_s(&vec[0], vec.size(), &iCode, sizeof(iCode));
			memcpy_s(&vec[sizeof(iCode)], vec.size() - sizeof(iCode), &strText[0], strText.length());
			return vec;
		}

		static std::pair<int, std::string> VectorToData(std::vector<unsigned char> &vec)
		{
			std::pair<int, std::string> p;
			memcpy_s(&p.first, sizeof(p.first), &vec[0], sizeof(p.first));
			p.second.resize(vec.size() - sizeof(p.first) + 1);
			memcpy_s(&p.second[0], p.second.size(), &vec[sizeof(p.first)], vec.size() - sizeof(p.first));
			return p;
		}

		Convert(void) = delete;
		~Convert(void) = delete;

		static std::string UnicodeToAnsi(const std::wstring&,
			int iLen = FullLength);

		static std::wstring AnsiToUnicode(const std::string&);

		static std::vector<BYTE> StringToBytes(const std::wstring&);
	};


	}