// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>
#include "TcpServer.h"
#include "TcpClient.h"
#include "TcpBase.h"
#include "NetException.h"
#include "Convert.h"
#include"CollectionOfDataAboutObject.h"





namespace GlobalData
{
	
	std::mutex mtx, mtxsecnd;

	std::vector<Example::TcpClient> vec(Example::Limits::CountConnected);

	void Work(const size_t &);
}


int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTitleW(L" Server ");
	try
	{
		Example::TcpBase::Init();
		Example::TcpServer server;
		server.Run();

		while (true)
		{
			try
			{
				Example::TcpClient client(server.Accept());
				GlobalData::mtx.lock();

				auto itFind = std::find_if(GlobalData::vec.begin(), GlobalData::vec.end(), [](const Example::TcpClient &t)
				{
					return !t.IsConnecting();
				});

				if (itFind == GlobalData::vec.cend())
					client.Disconnect();
				else
				{
					*itFind = std::move(client);
					std::thread th(GlobalData::Work, std::distance(GlobalData::vec.begin(), itFind));

					HANDLE hTmp = th.native_handle();
					th.detach();
					::CloseHandle(hTmp);

				}
				GlobalData::mtx.unlock();
			}
			catch (const Example::NetException & ex)
			{
				std::cout << ex.what() << std::endl;
			}
		}

	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}

void GlobalData::Work(const size_t & i)
{
	size_t index = i;
	try
	{
		std::string Name;
		while (true)
		{
			std::vector<unsigned char> vec(Example::Limits::MsgBuf);

			int sizeRecv = GlobalData::vec[index].Recv(vec,Name);
			if (sizeRecv < 0)
			{
				std::cout << " XXx " << std::endl;
			}
			vec.resize(sizeRecv);

			std::pair<int, std::string> p = Example::Convert::VectorToData(vec);

			std::ostringstream os;
			if (p.first == Example::CommandType::InRoom)
			{
				Name = p.second;
				os << " Name " << (GlobalData::vec[index].Name() = p.second);
			}
			else
				os << " Message from " << GlobalData::vec[index].Name() << " : " << p.second;

		//	std::cout << " vec number " << p.second << std::endl;
			os << p.second;
			std::string str = os.str();
			//str.resize(str.size() - 1);
			std::cout << str << std::endl;

			std::string answer(MAX_PATH, '\0');
	
			std::getline(std::cin, answer);

			GlobalData::mtx.lock();
			/*
			for ( auto & L : A)
			{
			vec = Example::Convert::DataToVector(Example::CommandType::Msg,
			L += '\n');

			GlobalData::vec[0].Send(vec);
			}*/
			//for (size_t i = 0; i < GlobalData::countClient; i++)      //GlobalData::vec.size(); i++)
		//	{
				//if (i != index && GlobalData::vec[i].IsConnecting())
				//{
				try
				{
					GlobalData::mtxsecnd.lock();
					/*for (auto & L : A)
					{*/
					/*vec = Example::Convert::DataToVector(Example::CommandType::Msg,
						L += '\n');*/

					vec = Example::Convert::DataToVector(Example::CommandType::Msg,
						answer += '\n');

					GlobalData::vec[i].Send(vec);
					//}
					GlobalData::mtxsecnd.unlock();
					//GlobalData::vec[i].Send(vec);
				}
				catch (Example::NetException)
				{
					continue;
				}
			//}
			///}
			GlobalData::mtx.unlock();
		}
	}

	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


