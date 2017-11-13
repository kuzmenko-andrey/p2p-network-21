#pragma once
#include"stdafx.h"
#include "TcpBase.h"

namespace Example
{

	class TcpClient : public TcpBase
	{
		std::string strName = std::string();
		//std::list <std::pair<int, std::string>>list;
		
		std::unique_ptr<std::list<std::pair<std::string,int>>> ulistPath;
		
		TcpClient(const TcpClient&) = delete;
		const TcpClient & operator = (const TcpClient&) = delete;
	public:
		TcpClient::TcpClient(const std::string &strName = strNameDefault,
			const int &iPort = Ports::Default);
	
		explicit TcpClient(SOCKET s) : TcpBase(s) {}
		TcpClient(TcpClient&&);
		const TcpClient & operator = (TcpClient&&);

		virtual ~TcpClient();

		void Connect(void);


		void Clear(void) noexcept;

		void Disconnect(void) noexcept;

		void FirstConnect(void);

		
		std::string& Name(void){
			return this->strName;
		}

		const std::string & Name(void) const{
			return this->strName;
		}

		const std::string & GetName(void) const{
			return this->strName;
		}
	
		void WriedPath(std::map<std::string, int>&);
	};

}