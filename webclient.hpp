#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "StringHelper.hpp"

namespace Web{
	namespace WebType {
		typedef boost::asio::ip::tcp::socket Http;
		typedef unsigned short u_short;
	}

	class WebClient
	{
	public:
		WebType::u_short port;
		std::string address;
	public:
		~WebClient() {
			Client.shutdown(boost::asio::socket_base::shutdown_both);
		};
		WebClient(WebType::u_short uPort) :port(uPort),Client(ioService){
			//...
		}
		void SetAddress(const std::string &ipAddress) {
			address = ipAddress;
		}
		std::string& GetAddress(){
			return address;
		}
		void Start() {
			boost::asio::ip::tcp::resolver resolver(ioService);
			boost::asio::ip::tcp::resolver::query query(address, std::to_string(port));
			boost::asio::ip::tcp::resolver::iterator Iter = resolver.resolve(query);
			boost::asio::connect(Client, Iter);
		};
		void Request(std::shared_ptr<boost::asio::streambuf> rBuffer) {

				std::size_t byteSize = 0;
				byteSize = boost::asio::write(Client, *rBuffer);
	
		};
		std::shared_ptr<boost::asio::streambuf> Response() {
			auto responseBuffer = std::make_shared<boost::asio::streambuf>();
			boost::system::error_code error;
			size_t headerlen = boost::asio::read(Client, *responseBuffer, boost::asio::transfer_all(), error);
			if (!error)
			{
				throw "this SOCKET is error";
			}
			return responseBuffer;
		};
	private:
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::endpoint point;
		boost::asio::ip::tcp::socket Client;
	};
}

