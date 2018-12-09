#pragma once

#include "webclient.hpp"
#include "StringHelper.hpp"
#include <regex>

namespace Web {

	//template<typename Net>
	class analyze
	{
	protected:
		std::shared_ptr<Web::WebClient> WebClient;
		std::wstring pattern;
		std::wstring Context;
		std::string ipAddress;
		std::string path;
		std::vector<std::wstring> stringBuffer;
	
	public:
		~analyze() {

		};
		//const std::shared_ptr<Net>
		analyze(std::shared_ptr<Web::WebClient>& Web) {
			WebClient = Web;
		}

		void Set(const std::wstring address,std::wstring patterns) {
			//...
			pattern = patterns;
			//判断是否是Http
			std::wstring http(L"http://");
			std::wstring https(L"https://");
			std::size_t hpos = address.find(http);
			std::size_t hspos = address.find(https);
			if (hpos != std::wstring::npos) {
				processUrl(http,address);

			}else if(hspos != std::wstring::npos){
				processUrl(https,address);
			}else{
				processPath(address);
			}
			
		};

		void run() {
			WebClient->Start();
			this->processRequest();
		};
		
		std::vector<std::wstring>& parseString() {
			std::wregex re(pattern);
			std::wsmatch result;
			while (std::regex_search(Context.cbegin(), Context.cend(), result, re))
			{
				stringBuffer.emplace_back(result[1].str());
				Context = result.suffix();
			}
			////out
			//for (auto str : stringBuffer)
			//{
			//	std::wcout << str << std::endl;
			//}
			return stringBuffer;
		};
	private:
		void processUrl(const std::wstring protocol,const std::wstring &address) {
			//判断 / 的substrpos
			std::size_t subStrPos = address.find(L"/", 9);
			if (subStrPos == std::wstring::npos) {
				path = "/";
				ipAddress = toStringUtf8(address.substr(protocol.length(), address.length()));
			}
			else {
				ipAddress = toStringUtf8(address.substr(protocol.length(), (subStrPos - protocol.length())));
				path = toStringUtf8(address.substr(subStrPos, address.length()));
			}
			WebClient->SetAddress(ipAddress);
		}
		void processPath(const std::wstring &oPath) {
			if (oPath.at(0) != L'/') {
				std::wstring tmp = L"/" + oPath;
				path = toStringUtf8(tmp);
			}
			else
			{
				path = toStringUtf8(oPath);
			}
		}
		void processRequest() {
			const std::string end("\r\n");
			auto requestBuffer = std::make_shared<boost::asio::streambuf>();
			std::ostream requestStream(requestBuffer.get());
			requestStream << "GET " << path << " HTTP/1.1" << end;
			requestStream << "Accept: " << "*/*" << end;
			requestStream << "Connection: " << "keep-alive" << end;
			requestStream << "Host: " << WebClient->address << end;
			//requestStream << "Accept-Encoding: " << "gzip" << end; 

			requestStream << end;
			WebClient->Request(requestBuffer);
			this->processResponse();
		}
		void processResponse() {
			std::stringstream sstream;
			std::shared_ptr<boost::asio::streambuf> streamBuffer =  WebClient->Response();
			std::istream stream(streamBuffer.get());
			stream >> sstream.rdbuf();
			Context = std::wstring(toStringUtf8(sstream.str()));
		}
	};
}