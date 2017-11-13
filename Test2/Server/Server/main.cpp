#include "stdafx.h"

namespace GlobalData{
	
	enum { ReturnInfo, ReturnFile };
	std::mutex mtx, mtxsecnd[Example::Limits::CountConnected];
	std::map<std::string, int>mapPath;
	std::vector<Example::TcpClient> vecClient(Example::Limits::CountConnected);
	
	void Work(const size_t &);
	void CheckPath(void);
}


void print_collection() {
	std::cout << " Seze " << GlobalData::mapPath.size() << " elements:\n";
	for (const auto & p : GlobalData::mapPath) {
		std::cout << "(" << p.first << "," << p.second << ")" << std::endl;;
	}
	std::cout << std::endl;
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

				auto itFind = std::find_if(GlobalData::vecClient.begin(), GlobalData::vecClient.end(), [](const Example::TcpClient &t)
				{
					return !t.IsConnecting();
				});

				if (itFind == GlobalData::vecClient.cend())
					client.Disconnect();
				else
				{
					*itFind = std::move(client);
					std::thread th(GlobalData::Work, std::distance(GlobalData::vecClient.begin(), itFind));

			//	HANDLE hTmp = th.native_handle();
					th.detach();
			//	::CloseHandle(hTmp);
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
	
	Example::MyCRITICAL criticl;
	size_t index = i;
	try
	{
		criticl.EnterCritical();
		GlobalData::vecClient[index].FirstConnect();
		GlobalData::vecClient[index].WriedPath(GlobalData::mapPath); // Запись всех путей что есть у клиента
		criticl.LeaveCritical();
		
		print_collection();

		GlobalData::mtxsecnd[index].lock();
		while (true)
		{
			std::vector<unsigned char> vec(Example::Limits::MsgBuf);
			int sizeRecv = GlobalData::vecClient[index].Recv(vec,"");
			vec.resize(sizeRecv);

			std::pair<int, std::string> p = Example::Convert::VectorToData(vec);			
			std::cout  << " Message from " << GlobalData::vecClient[index].Name() << " : " << p.second;
			std::cout << p.first << std::endl;
			if (p.first == ReturnInfo)
			{
				GlobalData::vecClient[index].Send(
					Example::Convert::DataToVector(ReturnInfo, Example::FunctionMain::GetPaths(mapPath, p.second)));
			}

		//	std::string answer(MAX_PATH, '\0');
			//std::getline(std::cin, answer);
		//	GlobalData::vecClient[index].Send(Example::Convert::DataToVector(0, answer));
		
		}
		GlobalData::mtxsecnd[index].unlock();
	}
	catch (const std::exception &ex)
	{
		criticl.EnterCritical();
		GlobalData::vecClient[index].Clear();
		std::cout << ex.what() << std::endl; // del
		CheckPath();
		print_collection(); // del
		criticl.LeaveCritical();
		GlobalData::mtxsecnd[index].unlock();
	}
}

void GlobalData::CheckPath(void)
{
	GlobalData::mapPath.clear();
	for (auto i = 0u; i < GlobalData::vecClient.size(); i++)
		GlobalData::vecClient[i].WriedPath(GlobalData::mapPath);
}



