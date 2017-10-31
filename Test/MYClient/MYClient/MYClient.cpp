// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>

#include "TcpServer.h"
#include "TcpClient.h"
#include "NetException.h"
#include "Convert.h"

namespace GlobalData
{
	Example::TcpClient client;

	void Work(void);
}

int _tmain(int argc, _TCHAR* argv[])
{

	try
	{

		GlobalData::client.Init();

	//	std::cout << "Type settings connect to server: 1 - default(localhost:33333) or 2 - manual:";
		std::string strVariant("1");
	//	std::getline(std::cin, strVariant);
		switch (std::stoi(strVariant))
		{
		case 1:
			break;
		case 2:
			
			GlobalData::client.Name() = "192.168.1.175";
			GlobalData::client.Port() = 33333;
			//std::cout << "Enter ip or name:";
			//std::getline(std::cin, GlobalData::client.Name());
			//std::cout << "Enter port:";
			//std::cin >> GlobalData::client.Port();
			break;
		default:
			throw std::exception("Invalid argument");
		}

		GlobalData::client.Connect();

		std::string strName;
		std::cout << "Enter user name:";
		std::getline(std::cin, strName);

		GlobalData::client.Send(Example::Convert::DataToVector(Example::CommandType::InRoom,
			strName));

		std::cout << "[Enter command exit to exit with chat]" << std::endl;

		std::thread th(GlobalData::Work);

		while (true)
		{
			std::string strText;
			std::getline(std::cin, strText);
			if (strText == "exit")
				break;
			GlobalData::client.Send(Example::Convert::DataToVector(Example::CommandType::Msg,
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

void GlobalData::Work(void)
{
	try
	{
		while (true)
		{
			std::vector<unsigned char> vec(Example::Limits::MsgBuf);

			int sizeRecv = GlobalData::client.Recv(vec);
			vec.resize(sizeRecv);
			if ((int)vec[0] != 0){
				//std::pair<int, std::string> p = Example::Convert::VectorToData(vec);
				std::cout << std::count(vec.begin(), vec.end(), '\n') << std::endl;

				std::string ins(MAX_PATH, '\0');
				for (auto i = 0u; i < vec.size(); i++)
				{
					if (vec[i] != '\0')
						std::cout << vec[i];

					if (vec[i] == '\n')
					{
						if (i < vec.size() - 1)
							std::cout << (int)vec[i + 1];
					}
				}
			}

		/*	if (p.first == Example::CommandType::Msg)
				std::cout << p.second << std::endl;
			std::cout << std::endl;*/
		}

	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}