#pragma once
#include "TcpBase.h"

namespace Example
{

	class TcpClient : public TcpBase
	{

		std::string strName = std::string();

		TcpClient(const TcpClient&) = delete;
		const TcpClient & operator = (const TcpClient&) = delete;
		
	public:

		TcpClient(TcpClient &&t) : TcpBase(t.strIp, t.iPort, t.s)
		{
			t.strIp.clear();
			t.iPort = 0;
			t.s = 0;
		}

		explicit TcpClient(SOCKET s) : TcpBase(s){}

		TcpClient(const std::string &strName = strNameDefault,
			const int &iPort = Ports::Default) : TcpBase(strName, iPort){}

		virtual ~TcpClient();

		void Connect(void);

		void Disconnect(void);

		const TcpClient & operator = (TcpClient && t)
		{
			if (this == &t)
				return *this;

			this->Disconnect();

			TcpBase::s = t.s;
			TcpBase::strIp = t.strIp;
			TcpBase::iPort = t.iPort;
			
			t.strIp.clear();
			t.iPort = 0;
			t.s = 0;

			return *this;
		}

		std::string & Name(void)
		{
			return this->strName;
		}

		const std::string & Name(void) const
		{
			return this->strName;
		}

	};

}