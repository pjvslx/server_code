/********************************************************************
Baina@copyright 2014
类名:
作用: 包含平台相关的基础头文件
特性:
作者: ???
时间:
---------------------------------------------------------------------
备注: 尽量避免引用这个头文件
*********************************************************************/
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define TARGET_OS_WINDOWS	1

#elif defined(ANDROID)
#define TARGET_OS_ANDROID	1

#elif defined(IOS)
#define TARGET_OS_IPHONE   1

#endif

// ANSI C .h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

//// STL .h
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <limits>
#include <set>

#ifdef _WIN32

#define strtoull _strtoui64
#define strtoll  _strtoi64

#ifndef snprintf
#define snprintf _snprintf
#endif

#endif

#ifdef _WIN32
// #define NOMINMAX
#define _CRT_SECURE_NO_DEPRECATE
#include <winsock2.h> // <winsock2.h> should be included in front of <windows.h>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")
#include <time.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h> 
#include <crtdbg.h>
#endif

#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/timeb.h>
#include <syslog.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>

#ifndef ANDROID
#include <sys/fcntl.h>
#else 
#include <fcntl.h>
#endif

#ifndef IOS
typedef unsigned int   size_t;
#endif

#endif
