#include "stdafx.h"
#include "TcpClient.h"

namespace Example
{
	TcpClient::TcpClient(const std::string& strName,
		const int& iPort)
		: TcpBase(strName, iPort),
		ulistPath(std::make_unique<std::list<std::pair<std::string,int>>>())
	{
	}

	TcpClient::TcpClient(TcpClient&& obj)
		: TcpBase(obj.strIp, obj.iPort, obj.s)
	{
		//this->strName = std::move(obj.strName);
		this->ulistPath = std::move(obj.ulistPath);
		obj.strIp.clear();
		obj.iPort = 0;
		obj.s = 0;
	}

	const TcpClient& TcpClient::operator = (TcpClient && obj) {
		if (this == &obj)
			return *this;

		this->Disconnect();

		TcpBase::s = obj.s;
		TcpBase::strIp = obj.strIp;
		TcpBase::iPort = obj.iPort;

		obj.strIp.clear();
		obj.iPort = 0;
		obj.s = 0;

		return *this;
	}


	TcpClient::~TcpClient() {
		std::cout << "DDD" << std::endl;
	}

	void TcpClient::Connect(void) {
		TcpBase::CreateSocket();

		struct sockaddr_in 	addr_server = { 0 };
		addr_server.sin_addr.s_addr = ::inet_addr(TcpBase::strIp.c_str()); // Convert 127.0.0.1 -> long / localhost
		if (INADDR_NONE == addr_server.sin_addr.s_addr) {
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
	void TcpClient::Disconnect(void) noexcept {
		TcpBase::CloseSocket();
	}

	void TcpClient::FirstConnect(void)
	{
		Example::MyCRITICAL criticl;
		std::vector<unsigned char> answerServer(1);
		std::vector<unsigned char> vec(Example::Limits::MsgBuf);

		criticl.EnterCritical();
		int size = this->Recv(vec, "");
		vec.resize(size);
		criticl.LeaveCritical();

		criticl.EnterCritical();
		std::pair<int, std::string> p = Example::Convert::VectorToData(vec);
		int sizePath = p.first;
		this->Name() =  p.second; //  Name 
		criticl.LeaveCritical();

		criticl.EnterCritical();
		this->Send(answerServer);
		criticl.LeaveCritical();

		for (auto i = 0; i < sizePath; i++)
		{
			std::vector<unsigned char> vecb(Example::Limits::MsgBuf);
			criticl.EnterCritical();
			size = this->Recv(vecb, "");
			vecb.resize(size);
			criticl.LeaveCritical();

			auto p1 = Example::Convert::VectorToData(vecb);
			(*this->ulistPath).push_back(std::pair<std::string,int>(p1.second, p1.first));

			criticl.EnterCritical();
			this->Send(answerServer);
			criticl.LeaveCritical();
		}
	}

	void TcpClient::WriedPath(std::map<std::string, int>& mpath){
		mpath.insert((*this->ulistPath).begin(),(*this->ulistPath).end());
	}

	void TcpClient::Clear(void) noexcept
	{
		this->Disconnect();
		this->strIp.clear();
		this->iPort = 0;
		this->s = 0;
		this->strName = std::string();
		(*this->ulistPath).clear();
	}

}