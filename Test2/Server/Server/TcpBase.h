#pragma once
#include"stdafx.h"
#include <Winsock.h>
#include <vector>
#include "NetException.h"
#pragma comment( lib, "Ws2_32.lib" )

namespace Example
{
	const std::string strNameDefault = "localhost";

	enum Ports {Default = 33333};

	enum CommandType{InRoom = 1, Msg, OutRoom};

	enum Limits{ MsgBuf = 4096, CountConnected = 10};

	class TcpBase
	{
		static bool bInit;
		static WSADATA wsd;
		
	protected:
		
		SOCKET s = 0;

		std::string strIp = std::string();
		int iPort = 0;

		TcpBase(void){}

		explicit TcpBase(SOCKET s) : s(s){}

		TcpBase(const std::string &strIp,
			const int &iPort,
			SOCKET s = 0) : strIp(strIp), iPort(iPort), s(s){}

		TcpBase(const TcpBase&) = delete;
		TcpBase(const TcpBase&&) = delete;
		const TcpBase & operator = (const TcpBase&) = delete;
		const TcpBase & operator = (TcpBase&&) = delete;

	public:

		virtual ~TcpBase();

		static void Init(void);
		
		virtual int Send(const std::vector<unsigned char> &) const;

		virtual int Recv(std::vector<unsigned char> &,const std::string&) const;

		std::string & Ip(void){
			return this->strIp;
		}

		const std::string & Ip(void) const throw(){
			return this->strIp;
		}

		const int & Port(void) const throw(){
			return this->iPort;
		}

		int & Port(void) throw(){
			return this->iPort;
		}

		bool IsConnecting(void)const throw(){
			return !(this->s == SOCKET_ERROR || this->s == INVALID_SOCKET || !this->s);
		}

	protected:

		void CreateSocket(void);

		void CloseSocket(void);
		
	};

}