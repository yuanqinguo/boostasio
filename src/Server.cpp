#include "Server.h"
#include <boost/bind.hpp>
#include <iostream>
#include <time.h>

Server::Server(const std::string& address, const std::string& port, const int iPoolSize)
	:m_io_AsioPool(iPoolSize)
	, m_work_AsioPool(1)
	,m_acceptor(m_io_AsioPool.GetIoService())
	,m_signalSet(m_work_AsioPool.GetIoService() )
{
	m_signalSet.add(SIGINT);
	m_signalSet.add(SIGTERM);
	m_signalSet.async_wait(boost::bind(&Server::HandleStop, this));

	boost::asio::ip::tcp::resolver resolver(m_acceptor.get_io_service());
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen(99999);

	OnAccept();

}

void Server::OnRun()
{
	m_io_thread.reset(new boost::thread(boost::bind(&AsioPool::OnRun, &m_io_AsioPool)));
	m_work_thread.reset(new boost::thread(boost::bind(&AsioPool::OnRun, &m_work_AsioPool)));
}

void Server::OnAccept()
{
	m_connectionPtr.reset(new Connection(m_io_AsioPool.GetIoService(), m_work_AsioPool.GetIoService(), m_RequstHandler));

	m_acceptor.async_accept(m_connectionPtr->GetSocket(),
		boost::bind(&Server::HandleAccept, this,
		boost::asio::placeholders::error) );
}

void Server::OnStop()
{
	m_io_AsioPool.OnStop();
	m_work_AsioPool.OnStop();

	m_io_thread->join();
	m_work_thread->join();
}

time_t GetCurrentStringTime(std::string& timeStr, int iIndex=0)
	{
		std::string tmpStr;
		char buf[32] = {0};

#ifdef _WIN32
		time_t t;
		tm *tp;
		t = time(NULL);
		tp = localtime(&t);
		sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", 
			tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday
			,tp->tm_hour, tp->tm_min, 
			(tp->tm_sec - iIndex > 0) ? tp->tm_sec - iIndex: 0);

		timeStr = buf;
		return t - iIndex;
#else
		time_t now;
		time(&now);
		now = now - iIndex;
		struct tm timenow;
		localtime_r(&now, &timenow);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timenow);
		timeStr = buf;
		return now;
#endif
	}

void Server::HandleAccept(const boost::system::error_code& errCode)
{
	OnAccept();
	static int iCount = 0;
	if (!errCode)
	{
		std::string timeStr;
		GetCurrentStringTime(timeStr);
		std::cout<<timeStr<<"::Server::HandleAccept::New Connection Coming--->iCount="<<iCount++<<std::endl;
		m_connectionPtr->OnStart();
	}
	
}

void Server::HandleStop()
{
	OnStop();
}
