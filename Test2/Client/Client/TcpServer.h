#pragma once
#include "TcpBase.h"

namespace Client
{
	class TcpServer : public TcpBase
	{
		int iCountConnected = 0;

		TcpServer(const TcpServer&) = delete;
		TcpServer(const TcpServer&&) = delete;
		const TcpServer & operator = (const TcpServer&) = delete;
		const TcpServer & operator = (TcpServer&&) = delete;

	public:

		TcpServer(const std::string &strName = std::string(),
			const int &iPort = Ports::Default,
			const int &iiCountConnected = Limits::CountConnected) 
			: TcpBase(strName, iPort), iCountConnected(iCountConnected){}

		virtual ~TcpServer();

		void Run(void);

		void Stop(void);

		SOCKET Accept(void)const;

	protected:
		void Bind(void);
		void Listen(void);
	};
}