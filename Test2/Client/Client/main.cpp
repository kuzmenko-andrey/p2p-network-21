#include "stdafx.h"

//namespace GlobalData
//{
//	enum{TEN = 10};
//	std::mutex mutexwork;
//	void Work(Client::TcpClient client);
//}
//
//
//int _tmainf(int argc, _TCHAR* argv[])
//{
//
//	SetConsoleTitleW(L" Client ");
//	try
//	{
//		//f();
//
//		Client::TcpClient client;
//		 client.Init();
//		
//	//	std::cout << "Type settings connect to server: 1 - default(localhost:33333) or 2 - manual:";
//		std::string strVariant("1");
//	//	std::getline(std::cin, strVariant);
//		switch (std::stoi(strVariant))
//		{
//		case 1:
//			break;
//		case 2:
//			
//			client.Name() = "192.168.1.175";
//			//std::cout << "Enter ip or name:";
//			//std::getline(std::cin, GlobalData::client.Name());
//			//std::cout << "Enter port:";
//			//std::cin >> GlobalData::client.Port();
//			break;
//		default:
//			throw std::exception("Invalid argument");
//		}
//		if (!client.Connect())
//		{
//			std::cout << " NO connect server " << std::endl;
//			std::cin.get();
//			exit(1);
//		}
//		GlobalData::Work(std::move(client));
//	}
//	catch(const std::exception& ex) {
//		std::cout << ex.what() << std::endl;
//	}
//	catch (...) {
//		std::cout << "Unknown exception" << std::endl;
//	}
//	return 0;
//}
//
//void GlobalData::Work(Client::TcpClient client)
//{
//	try
//	{
//		mutexwork.lock();
//		client.FirstConnect();
//		mutexwork.unlock();
//		/*while (true)
//		{
//			std::vector<unsigned char> vec(Client::Limits::MsgBuf);
//			int sizeRecv = client.Recv(vec);
//			vec.resize(sizeRecv);
//
//		}*/
//		std::cin.get();
//	}
//	catch (const std::exception &ex) {
//		std::cout << ex.what() << std::endl;
//	}
//}



namespace GlobalData{
	enum { ReturnInfo, ReturnFile };
	Client::TcpClient client;
	void Work(void);
}

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTitleW(L" Client ");
	Client::MyCRITICAL critical;
	try
	{
		GlobalData::client.Init();

		std::cout << "Type settings connect to server: 1 - default(localhost:33333) or 2 - manual:";
		std::string strVariant("1");
		//std::getline(std::cin, strVariant);
		switch (std::stoi(strVariant))
		{
		case 1:
			break;
		case 2:
			std::cout << "Enter ip or name:";
			std::getline(std::cin, GlobalData::client.Name());
			std::cout << "Enter port:";
			std::cin >> GlobalData::client.Port();
			break;
		default:
			throw std::exception("Invalid argument");
		}

		if (!GlobalData::client.Connect())
		{
			std::cout << " NO connect server " << std::endl;
			std::cin.get();
			exit(1);
		}
		
		critical.EnterCritical();
		GlobalData::client.FirstConnect();
		critical.LeaveCritical();

		std::thread th(GlobalData::Work);

		while (true)
		{
			std::string strText;
			std::getline(std::cin, strText);
			if (strText == "exit")
				break;
			GlobalData::client.Send(Client::Convert::DataToVector(GlobalData::ReturnInfo,
				strText));
		}
		GlobalData::client.Disconnect();
		th.join();
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return 0;
}
void Show(std::list<std::string>& l)
{
	std::cout << " size list " << l.size() << std::endl;
	for (const auto & i : l)
		std::cout << i << std::endl;

}
void GlobalData::Work(void)
{
	try
	{
		while (true)
		{
			std::vector<unsigned char> vec(Client::Limits::MsgBuf);
			int sizeRecv = GlobalData::client.Recv(vec);
			if (sizeRecv) {
				vec.resize(sizeRecv);
				std::pair<int, std::string> p = Client::Convert::VectorToData(vec);

				if (p.first == ReturnInfo)
				{
					Show(GlobalData::client.ServerAnswer(p.second));
				}
			}
		}
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}