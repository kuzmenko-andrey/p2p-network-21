#include "stdafx.h"
#include "TcpClient.h"

namespace Example
{
	TcpClient::~TcpClient()
	{
	}

	void TcpClient::Connect(void)
	{
		
		TcpBase::CreateSocket();

		struct sockaddr_in 	addr_server = { 0 };
		addr_server.sin_addr.s_addr = ::inet_addr(TcpBase::strIp.c_str()); // Convert 127.0.0.1 -> long / localhost
		if (INADDR_NONE == addr_server.sin_addr.s_addr)
		{
			hostent *h = ::gethostbyname(TcpBase::strIp.c_str()); // vk.com 
			if (!h)
				throw  Example::NetException("Error gethostbyname", ::WSAGetLastError());
				
			addr_server.sin_addr = *reinterpret_cast<in_addr*>(h->h_addr_list[0]);

		}

		addr_server.sin_family = AF_INET;
		addr_server.sin_port = ::htons(TcpBase::iPort);

		if (SOCKET_ERROR == ::connect(TcpBase::s,
			reinterpret_cast<sockaddr*>(&addr_server),
			sizeof(addr_server)))
			throw  Example::NetException("Error connect", ::WSAGetLastError());

	}

	void TcpClient::Disconnect(void)
	{
		TcpBase::CloseSocket();
	}

}