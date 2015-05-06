#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "AsioPool.h"
#include "Connection.h"
#include "RequestHandler.h"

class Server 
	:public boost::noncopyable
{
public:
	Server(const std::string& ip, const std::string& port, const int iPoolSize);

	void OnRun();
	
	void OnAccept();
	
	void OnStop();

	void HandleAccept(const boost::system::error_code& errCode);

	void HandleStop();

protected:
private:
	AsioPool m_io_AsioPool;
	AsioPool m_work_AsioPool;

	boost::asio::ip::tcp::acceptor m_acceptor;

	boost::asio::signal_set m_signalSet;

	typedef boost::shared_ptr<boost::thread> threadPtr;
	threadPtr m_io_thread;
	threadPtr m_work_thread;

	ConnectionPtr m_connectionPtr;

	RequestHandler m_RequstHandler;
};


#endif //SERVER_H_