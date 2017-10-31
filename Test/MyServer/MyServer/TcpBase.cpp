#include "stdafx.h"
#include "TcpBase.h"


namespace Example
{
	
	bool TcpBase::bInit = false;
	WSADATA TcpBase::wsd = {0};

	TcpBase::~TcpBase()
	{
		this->CloseSocket();
	}

	void TcpBase::Init(void)
	{
		if (!TcpBase::bInit)
		{
			if (::WSAStartup(MAKEWORD(2, 1), &TcpBase::wsd))
				throw Example::NetException("WSAStartup failed", ::WSAGetLastError());
			TcpBase::bInit = true;
		}
	}

	int TcpBase::Send(const std::vector<unsigned char> & vec) const
	{
		int iRet = ::send(this->s,
			reinterpret_cast<const char*>(&vec[0]),
			vec.size(),
			0);
		if (iRet == SOCKET_ERROR)
			throw  Example::NetException("Error send", ::WSAGetLastError());
		return iRet;
	}

	int TcpBase::Recv(std::vector<unsigned char> &vec,const std::string& pName) const
	{
		int iRet = ::recv(this->s,
			reinterpret_cast<char*>(&vec[0]),
			vec.size(),
			0);
		if (iRet == SOCKET_ERROR)
		{

			throw Example::NetException("Error recv", ::WSAGetLastError());
		}
			return iRet;
	}

	void TcpBase::CreateSocket(void)
	{
		this->CloseSocket();

		this->s = ::socket(AF_INET,
			SOCK_STREAM,
			0);
		if (this->s == SOCKET_ERROR)
			throw  Example::NetException("Error socket", ::WSAGetLastError());
	}

	void TcpBase::CloseSocket(void)
	{
		if (this->IsConnecting())
			::closesocket(this->s);
		this->s = 0;
	}

}