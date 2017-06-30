/********************************************************************
Baina@copyright 2014
类名: GException, GBackTraceException, LongJmpObject
作用: 提供exception相关的封装
特性:
作者: ???
时间:
---------------------------------------------------------------------
备注: 
*********************************************************************/
#ifndef __GEXCEPTION_H__
#define __GEXCEPTION_H__

#include <string>
#include <exception>
#include <setjmp.h>

class GException : std::exception
{
public:
    GException();
    virtual ~GException() throw();

    virtual void print() const;
};

class GBackTraceException : public GException
{
public:
    GBackTraceException();
    virtual ~GBackTraceException() throw();

    virtual void print() const;

public:
    void printBackTrace() const;
    const std::string backTrace() const;

protected:
    int m_nptrs;
    char **m_backtraces;
};

#define THROW_EXCEPTION(e) (throw e)

class LongJmpObject
{
public:
    LongJmpObject(const char *cur_file, int cur_line, const char *cur_fun) {};
    ~LongJmpObject() {};

public:
    static void initStatic() {};
    static void registerCurThread(const char* name) {};
    static void unRegisterCurThread() {};
};

#define TRY_BEGIN                                       \
    try{                                            \

#define TRY_END                                         \
}

#define CATCH_BEGIN(e)                                  \
    catch(e){                                       \

#define CATCH_END                                       \
}

#endif	//__GEXCEPTION_H__
