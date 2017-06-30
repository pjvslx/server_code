#include "GException.h"
#include <vector>

GException::GException() 
{

};

GException::~GException() throw()
{
};

void GException::print() const
{
    //WLOG("GException::Print()");
}

GBackTraceException::GBackTraceException()
{
    m_nptrs = 0;
    m_backtraces = NULL;
}

GBackTraceException::~GBackTraceException() throw()
{
    try
    {
        if (m_backtraces)
        {
            free(m_backtraces);
            m_backtraces = NULL;
        }
    }
    catch (...)
    {
        //WLOG("GBackTraceException::~GBackTraceException(): unknown exception");
    }
}

void GBackTraceException::printBackTrace() const
{
    if(true)
    {
        printf("GBackTraceException::PrintBackStrace:");
        if (m_backtraces)
        {
            for (int i = 0; i < m_nptrs; i++)
                printf(m_backtraces[i], NULL);
        }
    }
    else
    {
        printf("GBackTraceException::PrintBackStrace:");
        if (m_backtraces)
        {
            for (int i = 0; i < m_nptrs; i++)
                printf(m_backtraces[i], NULL);
        }
    }
}

void GBackTraceException::print() const
{
    printf("GBackTraceException::Print()");
    printBackTrace();
}

const std::string GBackTraceException::backTrace() const
{
    std::string err;
    err += "GBackTraceException::BackTrace:\n";
    if (m_backtraces)
    {
        for (int i = 0; i < m_nptrs; i++)
        {
            err += (m_backtraces[i]);
        }
    }
    return err;
}
