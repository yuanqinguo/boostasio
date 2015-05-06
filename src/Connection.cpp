#include <boost/bind.hpp>
#include <iostream>

#include "Connection.h"

Connection::Connection(boost::asio::io_service& ioservice,  boost::asio::io_service& work_service,
	RequestHandler& ReqHandler)
	: m_socket(ioservice)
	, m_work_service(work_service)
	, m_work_strand(work_service)
	, m_ReqHandler(ReqHandler)
{
	m_recvCacheBufPtr.reset(new std::vector<char>);
}

void Connection::OnStart()
{			
	boost::system::error_code gnoredErr;
	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);	
	// m_socket.async_read_some(boost::asio::buffer(m_Buffers), 
		// boost::bind(&Connection::HandleRead, shared_from_this(), 
		// boost::asio::placeholders::error, 
		// boost::asio::placeholders::bytes_transferred));

}

boost::asio::ip::tcp::socket& Connection::GetSocket()
{
	return m_socket;
}


void Connection::HandleRead(const boost::system::error_code& errCode, std::size_t bytes_transferred)
{
	if (!errCode)
	{
		int iret = 0;
		//m_ReqHandler,处理请求，比如登录等等
		if (!iret)
		{
			std::string timeStr;
			GetCurrentStringTime(timeStr);
			
			std::cout<<timeStr<<"::Connection::HandleRead::data = " << m_Buffers.data()<<std::endl;
			/*m_recvCacheBufPtr->resize(m_recvCacheBufPtr->size() + bytes_transferred);
			std::copy(m_Buffers.begin(), m_Buffers.begin() + bytes_transferred, 
				(m_recvCacheBufPtr->begin() + m_recvCacheBufPtr->size()));*/

			std::string reply = "Wirte";//回复OK

			boost::asio::async_write(m_socket, boost::asio::buffer(reply),
				boost::bind(&Connection::HandleWirte, shared_from_this(),
				boost::asio::placeholders::error)); 
			//m_work_strand.post(boost::bind(&Connection::HandleReadDone
			//	, shared_from_this(), errCode)); 
			
			// boost::asio::streambuf request;
			// std::ostream request_stream(&request);
			// request_stream << "Accept: */*\r\n";
			// request_stream << "Connection: close\r\n\r\n";

			// Send the request.
			//boost::asio::write(m_socket, request);

		}
		else if (-1 == iret)
		{
			m_socket.async_read_some(boost::asio::buffer(m_Buffers), 
				boost::bind(&Connection::HandleRead, shared_from_this(), 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred));
		}
		//else
		//{
		//	std::string reply;//回复失败
		//	boost::asio::async_write(m_socket, boost::asio::buffer(reply),
		//		boost::bind(&Connection::HandleWirte, shared_from_this(),
		//		boost::asio::placeholders::error));
		//}
	}
	else
	{
		std::string timeStr;
		GetCurrentStringTime(timeStr);
		std::cout<<timeStr<<"::Connection::HandleRead::Error shutdown" <<std::endl;
		boost::system::error_code gnoredErr;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);
	}
}

void Connection::HandleWirte(const boost::system::error_code& errCode)
{
	//if (!errCode)
	//{
	//	std::string timeStr;
	//	GetCurrentStringTime(timeStr);
	//	std::cout<<timeStr<<"::Connection::HandleWirte::write done" <<std::endl;
	//	m_socket.async_read_some(boost::asio::buffer(m_Buffers), 
	//		boost::bind(&Connection::HandleRead, shared_from_this(), 
	//		boost::asio::placeholders::error, 
	//		boost::asio::placeholders::bytes_transferred));
	//	////是否有其他需要处理，没有直接断掉链接
	//	//boost::system::error_code gnoredErr;
	//	//m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);
	//}
	//else
	if(!errCode)
	{
		std::string timeStr;
		GetCurrentStringTime(timeStr);
		std::cout<<timeStr<<"::Connection::HandleWirte::Wirte Done!!! shutdown socket" <<std::endl;
		boost::system::error_code gnoredErr;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);
	}
	else
	{
		std::string timeStr;
		GetCurrentStringTime(timeStr);
		std::cout<<timeStr<<"::Connection::HandleWirte::Wirte Error!!! shutdown socket" <<std::endl;
	}
	
	boost::system::error_code gnoredErr;
	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);
}

void Connection::HandleReadDone(const boost::system::error_code& errCode)
{
	if(!errCode)
	{
		std::string timeStr;
		GetCurrentStringTime(timeStr);
		std::string recvStr(m_recvCacheBufPtr->begin(), m_recvCacheBufPtr->end());
		std::cout<<timeStr<<"::Connection::HandleReadDone::m_recvCacheBufPtr = " << recvStr.c_str()<<std::endl;
		
		std::string reply = "Wirte";//回复OK

		boost::asio::async_write(m_socket, boost::asio::buffer(reply),
			boost::bind(&Connection::HandleWirte, shared_from_this(),
			boost::asio::placeholders::error));
	}
}