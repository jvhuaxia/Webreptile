//#include "webclient.hpp"
//#include "StringHelper.hpp"
//#include <boost/exception/all.hpp>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <codecvt>
#include "analyze.hpp"


int main() {

	system("chcp 65001");
	system("cls");
	std::wcout.imbue(std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>()));
	
	//rule
	{
		std::shared_ptr<Web::WebClient> wc = std::make_shared<Web::WebClient>(80);
		Web::analyze WebAnalyze(wc);
		WebAnalyze.Set(L"http://llss.mx/", L"<a class=\"top_nav\" href=\"(.*)\">");
		WebAnalyze.run();
		std::vector<std::wstring> strBuf = WebAnalyze.parseString();

		std::wcout << strBuf.at(0) << std::endl;

		Web::analyze WebAnalyze1(wc);
		WebAnalyze1.Set(strBuf.at(0),L"<a href=\"(.*)\" class=\"more-link\">");
		WebAnalyze1.run();
		std::vector<std::wstring> strBuf1 = WebAnalyze1.parseString();

		std::wcout << strBuf1.at(0) << std::endl;

		Web::analyze WebAnalyze2(wc);
		WebAnalyze2.Set(strBuf1.at(0), L"/>([0-9a-zA-Z].*)</p>");
		WebAnalyze2.run();
		std::vector<std::wstring> strBuf2 = WebAnalyze2.parseString();
		for (auto buf : strBuf2)
		{
			std::wcout << buf << std::endl;
		}
	}
	std::getchar();
	return 0;
}

//
//int main() {
//	system("chcp 65001");
//	system("cls");
//	
//	std::wcout.imbue(std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>()));
//	const std::string url("www.baidu.com");
//	const std::string Port("80");
//	const std::string end("\r\n");
//
//
//	boost::asio::io_service ioService;
//	boost::asio::ip::tcp::socket Client(ioService);
//
//	boost::asio::ip::tcp::resolver resolver(ioService);
//	boost::asio::ip::tcp::resolver::query query(url, Port);
//	boost::asio::ip::tcp::resolver::iterator Iter = resolver.resolve(query);
//	boost::asio::connect(Client, Iter);
//
//	auto requestBuffer = std::make_shared<boost::asio::streambuf>();
//	std::ostream requestStream(requestBuffer.get());
//
//	requestStream << "GET " << "/" << " HTTP/1.1" << end;
//	requestStream << "Accept: " << "*/*" << end;
//	////requestStream << "Accept-Encoding: " << "gzip" << end; 
//	requestStream << "Connection: " << "keep-alive"  << end;//"keep-alive"
//	requestStream << "Host: " << url << end;
//	requestStream << end;
//	std::size_t byteSize = 0;
//	byteSize = boost::asio::write(Client,*requestBuffer);
//
//	
//	auto responseBuffer = std::make_shared<boost::asio::streambuf>();
//	boost::system::error_code error;
//	size_t headerlen = boost::asio::read_until(Client, *responseBuffer, "\r\n\r\n");
//	//while (boost::asio::read(Client, *responseBuffer, boost::asio::transfer_at_least(1), error));
//	boost::asio::read(Client, *responseBuffer, boost::asio::transfer_all(), error);
//
//	std::istream stream(responseBuffer.get());
//	std::stringstream sstream;
//	stream >> sstream.rdbuf();
//	std::wstring wstr(toStringUtf8(sstream.str()));
//	size_t wlen = wstr.find(L"百度");
//	std::wcout << wstr << std::endl;
//	std::wcout << wlen << std::endl;
//
//	//wprintf(L"%s", wstr.c_str());
//	//
//	/*std::fstream file;
//	file.open("test.txt", std::ios::out|std::ios::binary);
//	file << responseBuffer.get();
//	file.flush();
//	file.close();*/
//	std::getchar();
//	return 0;
//}