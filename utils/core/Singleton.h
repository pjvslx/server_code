#pragma once

#ifndef ASSERT
#include <assert.h>
#define ASSERT(x) assert(x)
#endif

template <typename T> 
class Singleton
{
    static T* s_instance;

public:
    static T* newInstance() {
        return new T();
    }
    static void deleteInstance() {
        delete s_instance;
        s_instance = 0;
    }
    static T* instance() {
        return (s_instance);
    }

protected:
    // 禁止直接创建Singleton对象
    Singleton() {
        ASSERT(!s_instance && "the singleton object repeat construct");
        //int offset = (int)(T*) 1 - (int)(Singleton<T>*)(T*)1; // 计算派生类偏移
        //s_instance = (T*)((int)this + offset);
        s_instance = static_cast<T*>(this);
    }
    virtual ~Singleton() {
        ASSERT(s_instance && "the singleton object repeat destruct");
        s_instance = 0;
    }
private:
    Singleton(const Singleton& source) {} // 禁止copy构造

};

template<typename T>
T* Singleton<T>::s_instance = 0;
