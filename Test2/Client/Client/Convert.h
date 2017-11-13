#pragma once
#include <vector>
#include <string>

namespace Client
{
	class Convert
	{
	public:
		enum { FullLength = -1 };

		static std::vector<unsigned char> DataToVector(const int &iCode,
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

	 static	std::string Convert::UnicodeToAnsi(const std::wstring &wsParam,
			int iLen = FullLength)
		{

			std::string strReturn(iLen == FullLength ? wsParam.size() + 1 : iLen + 1, '\0');

			int iRet = ::WideCharToMultiByte(CP_ACP,
				WC_NO_BEST_FIT_CHARS,
				wsParam.c_str(),
				iLen,
				&strReturn[0],
				static_cast<int>(strReturn.size()),
				0,
				0);
			if (!iRet)
				throw Client::FileException("Error WideCharToMultiByte",
					::GetLastError());

			return strReturn.c_str();

		}
	};
}