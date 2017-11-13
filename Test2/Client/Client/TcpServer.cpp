#include "stdafx.h"
#include "TcpServer.h"

namespace Client
{

	TcpServer::~TcpServer(void)
	{
	}

	void TcpServer::Bind(void)
	{
		sockaddr_in local_addr = { 0 };
		local_addr.sin_family = AF_INET;
		local_addr.sin_port = ::htons(TcpBase::iPort);
		local_addr.sin_addr.s_addr = TcpBase::strIp.length() ? ::inet_addr(TcpBase::strIp.c_str()) : 0;

		if (::bind(TcpBase::s, reinterpret_cast<sockaddr*>(&local_addr), sizeof(local_addr)) == SOCKET_ERROR)
			throw NetException("Error bind", ::WSAGetLastError());
	}

	void TcpServer::Listen(void)
	{
		if (::listen(TcpBase::s, this->iCountConnected) == SOCKET_ERROR)
			throw NetException("Error listen", ::WSAGetLastError());
	}

	void TcpServer::Run(void) {
		TcpBase::CreateSocket();
		this->Bind();
		this->Listen();
	}

	void TcpServer::Stop(void) {
		TcpBase::CloseSocket();
	}

	SOCKET TcpServer::Accept(void)const
	{
		sockaddr_in client_addr = { 0 };
		int client_addr_size = sizeof(client_addr);

		SOCKET s = ::accept(TcpBase::s,
			reinterpret_cast<sockaddr*>(&client_addr),
			&client_addr_size);
		if (s == INVALID_SOCKET)
			throw NetException("Error accept", ::WSAGetLastError());
		return s;

	}
}