#include "stdafx.h"
#include "TcpBase.h"


namespace Client
{
	bool TcpBase::bInit = false;
	WSADATA TcpBase::wsd = {0};

	TcpBase::~TcpBase(){
		this->CloseSocket();
	}

	void TcpBase::Init(void)
	{
		if (!TcpBase::bInit)
		{                   // (MAKEWORD(2, 0)
			if (::WSAStartup(MAKEWORD(1, 1), &TcpBase::wsd))
				throw NetException("WSAStartup failed", ::WSAGetLastError());
			TcpBase::bInit = true;
		}
	}

	int TcpBase::Send(const std::vector<unsigned char> &vec) const
	{
		int iRet = ::send(this->s,
			reinterpret_cast<const char*>(&vec[0]),
			vec.size(),
			0);
		if (iRet == SOCKET_ERROR)
			throw NetException("Error send", ::WSAGetLastError());
		return iRet;
	}

	int TcpBase::Recv(std::vector<unsigned char> &vec) const
	{
		int iRet = ::recv(this->s,
			reinterpret_cast<char*>(&vec[0]),
			vec.size(),
			0);
		if (iRet == SOCKET_ERROR)
			throw NetException("Error recv", ::WSAGetLastError());
		return iRet;
	}

	void TcpBase::CreateSocket(void)
	{
		this->CloseSocket();

		this->s = ::socket(AF_INET,
			SOCK_STREAM,
			0);
		if (this->s == SOCKET_ERROR)
			throw NetException("Error socket", ::WSAGetLastError());
	}

	void TcpBase::CloseSocket(void)
	{
		if (this->IsConnecting())
			::closesocket(this->s);
		this->s = 0;
	}

}