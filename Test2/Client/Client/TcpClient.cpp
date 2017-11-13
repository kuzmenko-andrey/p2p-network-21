#include "stdafx.h"
#include "TcpClient.h"

namespace Client
{
	TcpClient::TcpClient(const std::string &strName,
		const int &iPort)
		: TcpBase(strName, iPort),
		vec(std::vector<unsigned char>(Client::Limits::MsgBuf)),
	mapPath(std::map<std::string, char>())
	{
	}

	TcpClient::TcpClient(TcpClient&& obj)
		: TcpBase(obj.strIp, obj.iPort, obj.s),
		vec(std::move(obj.vec)),
		mapPath(std::move(mapPath))
	{
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

		this->vec = std::move(obj.vec);
		mapPath = std::move(mapPath);

		obj.strIp.clear();
		obj.iPort = 0;
		obj.s = 0;

		return *this;
	}


	TcpClient::~TcpClient()
	{
	}
	bool TcpClient::Connect(void)
	{
		TcpBase::CreateSocket();

		struct sockaddr_in 	addr_server = { 0 };
		addr_server.sin_addr.s_addr = ::inet_addr(TcpBase::strIp.c_str()); // Convert 127.0.0.1 -> long / localhost
		if (INADDR_NONE == addr_server.sin_addr.s_addr)
		{
			hostent *h = ::gethostbyname(TcpBase::strIp.c_str()); // vk.com 
			if (!h)
				throw NetException("Error gethostbyname", ::WSAGetLastError());

			addr_server.sin_addr = *reinterpret_cast<in_addr*>(h->h_addr_list[0]);
		}

		addr_server.sin_family = AF_INET;
		addr_server.sin_port = ::htons(TcpBase::iPort);

		auto iconnect = ::connect(TcpBase::s,
			reinterpret_cast<sockaddr*>(&addr_server),
	        sizeof(addr_server));

		if (SOCKET_ERROR == iconnect)
			return false;
		return true;
	}

	void TcpClient::Disconnect(void) {
		TcpBase::CloseSocket();
	}

	void TcpClient::FirstConnect(void)
	{
		std::vector<unsigned char> vec(Client::Limits::MsgBuf);
		std::vector<unsigned char> answerServer(1);

		Client::ListPath listpaths;
		auto lpaths = listpaths.File_path_list(this->cornerSrorege);
		
		this->criticalClient.EnterCritical();
		this->Send(Client::Convert::DataToVector(lpaths.size(),this->NameComp()));
		this->criticalClient.LeaveCritical();

		this->criticalClient.EnterCritical();
	    this->Recv(answerServer);
		this->criticalClient.LeaveCritical();

		
		for (const auto & i : lpaths){
			this->criticalClient.EnterCritical();
			this->mapPath.insert(std::make_pair(i.second, 0));
			this->Send(Client::Convert::DataToVector(i.first, i.second));
			this->criticalClient.LeaveCritical();

			this->criticalClient.EnterCritical();
			this->Recv(answerServer);
			this->criticalClient.LeaveCritical();
		}
	}


	std::list<std::string>TcpClient::ServerAnswer(std::string& paths)const
	{
		std::string str;
		std::list<std::string>buf;
		while (auto i = paths.find('/'))
		{
			if (i == std::string::npos)
				break;
		    str = paths.substr(0, i);
			std::cout << "return " << str << std::endl;
			
			if(this->mapPath.find(str.substr(0, str.length() - ONE)) == this->mapPath.end())
			   buf.push_back(str);
			paths.erase(0, ++i);
		}

		if (this->mapPath.find(paths.substr(0, paths.length() - ONE)) == this->mapPath.end())
			buf.push_back(paths);

		return std::move(buf);
	}

	


	std::string TcpClient::NameComp() const
	{
		DWORD size = MAX_PATH;
		std::wstring comName(size, '\0');
		GetComputerName(&comName[0], &size);
		return Convert::UnicodeToAnsi(comName);
	}
}