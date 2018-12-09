#pragma once

#include <string>



#define _Win32_

#ifdef _Win32_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
std::wstring toStringUtf8(const std::string& str) {
	std::size_t len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wTmp = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wTmp, len);
	std::wstring newStr(wTmp);
	delete[] wTmp;
	return newStr;
}

std::string toStringUtf8(const std::wstring& str) {
	std::size_t len = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char *cTmp = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, cTmp, len, NULL, NULL);
	std::string newStr(cTmp);
	delete[] cTmp;
	return newStr;
}
#else

#endif
