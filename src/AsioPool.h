#ifndef ASIOPOOL_H_
#define ASIOPOOL_H_

#include <boost/asio.hpp>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>


class AsioPool 
	:private boost::noncopyable
{
public:
	AsioPool(int iPoolSize);

	void OnRun();

	void OnStop();

	boost::asio::io_service& GetIoService(); 
protected:

private:
	typedef boost::shared_ptr<boost::asio::io_service> IOServicePtr;
	typedef boost::shared_ptr<boost::asio::io_service::work> IOWorkPtr;

	std::vector<IOServicePtr> m_ioServicePtrs;
	std::vector<IOWorkPtr> m_ioWorkPtrs;

	size_t m_ioIndex;
};

#endif//ASIOPOOL_H_