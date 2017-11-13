#pragma once
#include <exception>
#include <string>
#include <sstream>

namespace Client
{
	class NetException : public std::exception
	{
		int iCode = 0;
		std::string strMsg;
	public:
		//NetException();

		NetException(const std::string &strMsg,
			const int &iCode) : exception(strMsg.c_str()), iCode(iCode)
		{
			std::ostringstream os;
			os << exception::what() << ", error code = " << this->iCode;
			this->strMsg = os.str();
		}

		virtual ~NetException(void){ }

		const char * what(void)const override
		{
			return this->strMsg.c_str();
		}
	};
	class FileException : public NetException
	{
	public:
		FileException(const char * str, int dw) : NetException(str, dw) {}
		virtual~FileException() {}
	};
}