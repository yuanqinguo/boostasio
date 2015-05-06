#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  //try
  {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "192.168.199.146", "12345");
    tcp::resolver::iterator iterator = resolver.resolve(query);

	while(1)
    {	
		tcp::socket s(io_service);
		boost::asio::connect(s, iterator);
		boost::system::error_code gnoredErr;
		s.shutdown(boost::asio::ip::tcp::socket::shutdown_both, gnoredErr);	
	}

    // using namespace std; // For strlen.
    // std::cout << "Enter message: ";
    // char request[max_length];
    // std::cin.getline(request, max_length);
    // size_t request_length = strlen(request);
    // boost::asio::write(s, boost::asio::buffer(request, request_length));

    // char reply[max_length];
    // size_t reply_length = boost::asio::read(s,
        // boost::asio::buffer(reply, request_length));
    // std::cout << "Reply is: ";
    // std::cout.write(reply, reply_length);
    // std::cout << "\n";
  }
  //catch (std::exception& e)
  {
    //std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}