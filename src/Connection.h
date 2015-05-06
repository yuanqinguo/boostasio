#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "RequestHandler.h"

#include <sstream>
#include <fstream>
#include <iomanip>

#include <string>
#include <vector>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <ObjBase.h>
#include <direct.h>
#include <io.h>

#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define SLEEP(n) Sleep(1000*n)


#else
#include <uuid/uuid.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>

#define ACCESS access
#define MKDIR(a) mkdir((a),0766)
#define SLEEP(n) sleep(n)

#endif

using namespace std;


class Connection 
	:public boost::enable_shared_from_this<Connection>
	,private boost::noncopyable
{
public:
	Connection(boost::asio::io_service& ioservice,  boost::asio::io_service& work_service, RequestHandler& ReqHandler);

	void OnStart();

	boost::asio::ip::tcp::socket& GetSocket();

protected:
	void HandleRead(const boost::system::error_code& errCode, std::size_t bytes_transferred);
	void HandleReadDone(const boost::system::error_code& errCode);

	void HandleWirte(const boost::system::error_code& errCode);

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
private:

	boost::asio::ip::tcp::socket m_socket;
	boost::asio::io_service& m_work_service;

	boost::array<char, 8192> m_Buffers;

	boost::shared_ptr<std::vector<char> > m_recvCacheBufPtr; 

	boost::asio::strand m_work_strand;

	RequestHandler& m_ReqHandler;

};

typedef boost::shared_ptr<Connection> ConnectionPtr;

#endif //CONNECTION_H_