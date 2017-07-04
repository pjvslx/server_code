/********************************************************************
Baina@copyright 2014
类名: DoubleBufferQueue
作用: 双缓冲队列，为ClientSession提供接口
特性:
作者: ???
时间:
---------------------------------------------------------------------
备注: 
*********************************************************************/
#pragma once

#include <list>
#include <assert.h>
#include <stdlib.h>

#include "Lock.h"

template <typename T, void (*free_hook)(T&), size_t MAX_SIZE = 128>
class DoubleBufferQueue
{
private:
    class SwapBuffer
    {
    public:
        SwapBuffer(size_t max_size) : m_rPos(0), m_wPos(0), m_maxSize(max_size), m_c(NULL)
        {
            m_c = new T[m_maxSize];
        };

        ~SwapBuffer()
        {
            clear();
            if (m_c != NULL)
            {
                delete[] m_c;
                m_c = NULL;
            }
        };

        void clear()
        {
            if (free_hook)
            {
                while (m_rPos < m_wPos)
                {
                    free_hook(m_c[m_rPos++]);
                }
            }
            m_rPos = m_wPos = 0;
        };

        bool isFull() { return m_wPos >= m_maxSize; };
        bool isEmpty() { return m_rPos >= m_wPos; };
        size_t size() { return m_wPos - m_rPos; };

        T get() 
        {
            assert(m_wPos > m_rPos);
            return m_c[m_rPos++];
        };
        bool put(T p)
        {
            if (m_wPos < m_maxSize)
            {
                m_c[m_wPos++] = p;
                return true;
            }
            return false;
        };

    private:
        T *m_c;
        size_t m_rPos, m_wPos;
        size_t m_maxSize;
    };

public:
    DoubleBufferQueue(size_t buffer_size = MAX_SIZE) : m_bufferSize(buffer_size)
    { 
        m_c1 = new SwapBuffer(m_bufferSize);
        m_c2 = new SwapBuffer(m_bufferSize);
        m_pReadBuffer = m_c1;
        m_pWriteBuffer = m_c2;
    };

    ~DoubleBufferQueue()
    {
        clear();
        if (m_c1) {
            delete m_c1;
            m_c1 = NULL;
        }
        if (m_c2) {
            delete m_c2;
            m_c2 = NULL;
        }
    };

    void clear()
    {
		mdk::AutoLock lock(&m_mutex);
        m_pReadBuffer->clear();
        m_pWriteBuffer->clear();
        while(!m_cs.empty())
        {
            SwapBuffer *p = m_cs.front();
            p->clear();
            if (p != m_c1 && p != m_c2)
            {
                delete p;
            }
            m_cs.pop_front();
        }
    };

    size_t swap()
    { 
        if (m_pReadBuffer->isEmpty())
        {
			mdk::AutoLock lock(&m_mutex);
            if (!m_cs.empty())
            {
                if (m_pReadBuffer != m_c1 && m_pReadBuffer != m_c2)
                {
                    delete m_pReadBuffer;
                }
                else
                {
                    m_cs.push_back(m_pWriteBuffer);
                    m_pWriteBuffer = m_pReadBuffer;
                    m_pWriteBuffer->clear();
                }
                m_pReadBuffer = m_cs.front();
                m_cs.pop_front();
            }
            else if (!m_pWriteBuffer->isEmpty())
            {
                SwapBuffer* t = m_pReadBuffer;
                m_pReadBuffer = m_pWriteBuffer;
                m_pWriteBuffer = t;
                m_pWriteBuffer->clear();
            }
        }
        return m_pReadBuffer->size();
    }

    bool isEmpty()
    {
        return m_pReadBuffer->isEmpty();
    };

    T get() 
    {
        return m_pReadBuffer->get();
    };
    bool canPut()
    {
        return !(m_pWriteBuffer->isFull());
    };
    bool put(T p)
    {
        bool s = false;
		mdk::AutoLock lock(&m_mutex);
        if (m_pWriteBuffer->isFull())
        {
            m_cs.push_back(m_pWriteBuffer);
            m_pWriteBuffer = new SwapBuffer(m_bufferSize);
        }
        s = m_pWriteBuffer->put(p);
        return s;
    };

private:
    SwapBuffer *m_c1;
    SwapBuffer *m_c2;
    std::list<SwapBuffer*> m_cs;

    SwapBuffer *m_pReadBuffer;
    SwapBuffer *m_pWriteBuffer;

    size_t m_bufferSize;
    //LOCK m_lock;
	mdk::Mutex m_mutex;
};
