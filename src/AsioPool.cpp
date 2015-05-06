#include "AsioPool.h"

#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

AsioPool::AsioPool(int iPoolSize)
{
	m_ioIndex = 0;
	if (iPoolSize <= 0)
	{
		iPoolSize = 1;
	}

	for (int i=0; i<iPoolSize; i++)
	{
		IOServicePtr ioservice(new boost::asio::io_service);
		IOWorkPtr iowork(new boost::asio::io_service::work(*ioservice));

		m_ioServicePtrs.push_back(ioservice);
		m_ioWorkPtrs.push_back(iowork);
	}
}

void AsioPool::OnRun()
{
	typedef boost::shared_ptr<boost::thread> threadPtr;
	std::vector<threadPtr> threads;
	size_t i,j;

	for (i=0; i<m_ioServicePtrs.size(); i++)
	{
		threadPtr thread(new boost::thread( 
			boost::bind(&boost::asio::io_service::run, m_ioServicePtrs.at(i)) ) );

		threads.push_back(thread);
	}

	for (j=0; j<threads.size(); j++)
	{
		threads.at(j)->join();
	}
}

void AsioPool::OnStop()
{
	size_t i;
	for (i=0; i<m_ioServicePtrs.size(); i++)
	{
		m_ioServicePtrs.at(i)->stop();
	}
}

boost::asio::io_service& AsioPool::GetIoService()
{
	boost::asio::io_service& service = *m_ioServicePtrs.at(m_ioIndex);
	if (++m_ioIndex >= m_ioServicePtrs.size())
	{
		m_ioIndex = 0;
	}
	return service;
}