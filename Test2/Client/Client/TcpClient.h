#pragma once
#include "stdafx.h"
#include "TcpBase.h"
#include"MyCRITICAL.h"
namespace Client
{
	
	class TcpClient : public TcpBase
	{
		enum {ONE = 1,TWO};
		const std::string cornerSrorege = "..\\Storage";
		Client::MyCRITICAL criticalClient,firstconnect;
		

		std::vector<unsigned char> vec;
	
		std::string strName = std::string();
		std::map<std::string,char>mapPath;

		TcpClient(const TcpClient&) = delete;
		const TcpClient & operator = (const TcpClient&) = delete;
	public:
		TcpClient::TcpClient(const std::string& strName = strNameDefault,
			const int& iPort = Ports::Default);

		explicit TcpClient(SOCKET s) : TcpBase(s) {}
		TcpClient(TcpClient&&);
		const TcpClient & operator = (TcpClient&&);

		virtual ~TcpClient();

		bool Connect(void);
		void Disconnect(void);
		void FirstConnect(void);
		
		std::list<std::string> ServerAnswer(std::string&)const;
		std::string NameComp()const;

	std::string& Name(void) {
			return this->strName;
		}

		const std::string & Name(void) const {
			return this->strName;
		}
	};
}