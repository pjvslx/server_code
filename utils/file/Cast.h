/********************************************************************
Baina@copyright 2014
类名:
作用: 提供一些类型转换、编码格式切换，提供std::format、std::fromatv的实现，以及对大数的处理（如10000转为1万）
特性:
作者: ???
时间:
---------------------------------------------------------------------
备注: 
*********************************************************************/
#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include <stdlib.h>
#include <string>
#include <vector>
#include "TypeDef.h"

#ifndef QUICK_COCOS2D
#pragma execution_character_set("utf-8")
#endif

#define array_sizeof(a) (sizeof(a)/sizeof(a[0]))

//char *数组转为wchar_t *数组
wchar_t* Cstr2Wstr(const char *pcstr, wchar_t *pwstr);
//char string转为wchar_t string
std::wstring Cstr2Wstr(const char *pcstr);

//Special function to convert string in database for auto_line_feed and non-break-spaces
const char * FilterString(const char *inStr, int &outLen, bool isFrench = false);

//UTF8转为Unicode
bool Utf82Unicode(const char* inStr, wchar_t* outStr, int buffLen);
bool Utf82Unicode(const char* inStr, const int inLen, wchar_t* outStr, int& outLen);

//Unicode转为UTF8
bool Unicode2Utf8(const wchar_t* inStr, char* outStr, int buffLen);
bool Unicode2Utf8(const wchar_t* inStr, const int inLen, char* outStr, int& outLen);

//计算中文字符数量
int CountStringCN2EU1_utf8(const char* inStr);
int CountStringCN2EU1_unicode(const wchar_t* inStr);

//判断中文字符字节数是否大于limitLen
bool LimitStringCN2EU1_utf8(char *inStr, int limitLen);
bool LimitStringCN2EU1_unicode(wchar_t *inStr, int limitLen);

inline std::string& operator +(std::string& s, int v)
{
    char szValue[12] = { 0 };
    sprintf(szValue, "%d", v);
    s += szValue;
    return s;
}

inline std::string& operator +(std::string& s, unsigned int v)
{
    char szValue[12] = { 0 };
    sprintf(szValue, "%u", v);
    s += szValue;
    return s;
}

inline std::string operator +(const char * lpszText1, const std::string& text2)
{
    std::string result(lpszText1);
    result += text2;
    return result;
}

namespace std
{
    std::string format(const char * fmt, ...) FORMAT_FUNC(1, 2);
    std::string formatv(const char * fmt, va_list ap);
#ifndef IOS
    std::wstring format(const wchar_t * fmt, ...);
    std::wstring formatv(const wchar_t * fmt, va_list ap);
#endif
}

#if defined(_WIN32) || defined(_WIN64)

int ConvertEncoding(UINT nSourceCP, const char * lpszText, UINT nDestCP, char * lpszOutput, int cbOutput);
int AnsiToUTF8(const char * lpszText, char * lpszOutput, int cbOutput);
int UTF8ToAnsi(const char * lpszText, char * lpszOutput, int cbOutput);

std::string UnicodeToAnsi(const std::wstring& text, UINT nCodePage = CP_ACP);
std::wstring AnsiToUnicode(const std::string& text, UINT nCodePage = CP_ACP);
std::string ConvertEncoding(UINT nSourceCP, const std::string& text, UINT nDestCP);
std::string AnsiToUTF8(const std::string& text);
std::string UTF8ToAnsi(const std::string& text);

#endif

//分解筹码数额
std::vector<int> CurrencyDecomposition(int number, int maxUnit);

