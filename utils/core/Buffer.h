/********************************************************************
Baina@copyright 2014
类名: BufferException, IStreamBuffer, CBuffer, FileBuffer
作用: 提供IStreamBuffer类做流式buffer操作，提供CBuffer模板，提供FileBuffer类。
特性:
作者: ???
时间:
---------------------------------------------------------------------
备注: 
*********************************************************************/
#pragma once
#include "PlatformMacros.h"
#include "GException.h"
#include "TypeDef.h"
#ifdef IPHONEOS
#include <math.h>
#endif

#ifdef ANDROID
#include <cmath>
#endif

class BufferException : public GBackTraceException
{
public:
    BufferException() {}
    virtual ~BufferException() throw() {}
};

enum IStreamBufferMode {
    IStreamBufferRead = 1 << 0,
    IStreamBufferWrite = 1 << 1,
    IStreamBufferReadAndWrite = IStreamBufferRead | IStreamBufferWrite,
};

class IStreamBuffer
{
    friend class BufferException;
    friend class CBitMask;

public:
    IStreamBuffer():m_buffSize(0), m_pBuff(NULL), m_validateSize(0), m_rPos(0), m_wPos(0), m_mode(IStreamBufferReadAndWrite)
    {

    }

    virtual ~IStreamBuffer()
    { 
        __clear();
    };

    IStreamBuffer& operator= (const IStreamBuffer& r)
    {
        if (this != &r)
        {
            clear();
            _resize(r.m_validateSize);
            m_rPos = r.m_rPos;
            m_wPos = r.m_wPos;
            memcpy(m_pBuff, r.m_pBuff, r.m_validateSize);
        }
        return *this;
    };

public:
    virtual void clear()
    {
        __clear();
    };

private:
    virtual void __resize(size_t newsize) = 0;

private:
    void __clear()
    {
        m_validateSize = m_rPos = m_wPos = 0;
        m_pBuff = NULL;
        m_buffSize = 0;
    };

public:
    uint8* getContent() const      { return m_pBuff; }

    uint8* getWriteBuffer(size_t size)
    {
        size_t pos = m_wPos;
        m_wPos += size;
        _resize(m_wPos);
        return m_pBuff + pos;
    }
    uint8* getReadBuffer() const
    {
        //size_t pos = _rpos;
        //_rpos += size;
        return m_pBuff + m_rPos;
    };

    size_t getRemainSize() const { return m_validateSize - m_rPos; }
    size_t getSize() const       { return m_validateSize; }
    size_t bufferSize() const    { return m_buffSize; }

    void skipRead(size_t size)   { m_rPos += size; };
    void resetPosRead()          { m_rPos = 0; }; //ATTENTION: please use this carefully, MS use the rpos to improve performance, any question please contact zhuj
    void skipToEnd()             { m_rPos = m_validateSize; };
    size_t getReadPos() const    { return m_rPos; }

    void append(const char *src, uint16 size) { append((const uint8 *)src, size); }
    void append(const uint8 *src, uint16 size) { if (size > 0) _write(src, size); }

    size_t getWritePos() const   { return m_wPos; }

    bool canRead() const         { return m_validateSize - m_rPos > 0; }

    //static const size_t MAXSIZE = 0xffffffff;//0xffff;
    void setBufferMode(IStreamBufferMode mode) { m_mode = mode; }

    IStreamBufferMode getBufferMode()       { return m_mode; }

    IStreamBuffer& operator<<(char value)   { _write<char>(value); return *this; }
    IStreamBuffer& operator>>(char &value)  { _read<char>(value); return *this; }

    IStreamBuffer& operator<<(bool value)   { _write<bool>(value); return *this; }
    IStreamBuffer& operator>>(bool &value)  { _read<bool>(value); return *this; }

    IStreamBuffer& operator<<(int8 value)   { _write<int8>(value); return *this; }
    IStreamBuffer& operator>>(int8 &value)  { _read<int8>(value); return *this; }
    IStreamBuffer& operator<<(uint8 value)  { _write<uint8>(value); return *this; }
    IStreamBuffer& operator>>(uint8 &value) { _read<uint8>(value); return *this; }

    IStreamBuffer& operator<<(int16 value)  { _write<int16>(_BITSWAP16(value)); return *this; }
    IStreamBuffer& operator>>(int16 &value)	{ _read<int16>(value); value = _BITSWAP16(value); return *this; }
    IStreamBuffer& operator<<(uint16 value)	{ _write<uint16>(_BITSWAP16(value)); return *this; }
    IStreamBuffer& operator>>(uint16 &value){ _read<uint16>(value); value = _BITSWAP16(value); return *this; }

    IStreamBuffer& operator<<(int32 value)  { _write<int32>(_BITSWAP32(value)); return *this; }
    IStreamBuffer& operator>>(int32 &value)	{ _read<int32>(value); value = _BITSWAP32(value); return *this; }
    IStreamBuffer& operator<<(uint32 value)	{ _write<uint32>(_BITSWAP32(value)); return *this; }
    IStreamBuffer& operator>>(uint32 &value){ _read<uint32>(value); value = _BITSWAP32(value); return *this; }

    IStreamBuffer& operator<<(int64 value)  { _write<int64>(_BITSWAP64(value)); return *this; }
    IStreamBuffer& operator>>(int64 &value)	{ _read<int64>(value); value = _BITSWAP64(value); return *this; }
    IStreamBuffer& operator<<(uint64 value)	{ _write<uint64>(_BITSWAP64(value)); return *this; }
    IStreamBuffer& operator>>(uint64 &value){ _read<uint64>(value); value = _BITSWAP64(value); return *this; }

    // size_t differs in 32/64 bit
//#ifdef IOS
//    IStreamBuffer& operator<<(size_t value) { _write<size_t>(_BITSWAP64(value)); return *this; }
//    IStreamBuffer& operator>>(size_t &value){ _read<size_t>(value); value = _BITSWAP64(value); return *this; }
//#endif

    IStreamBuffer& operator<<(const char *value);
    //IStreamBuffer& operator>>(char *value);

    IStreamBuffer& operator<<(const std::string &value);
    IStreamBuffer& operator>>(std::string &value);

    IStreamBuffer& operator<<(const IStreamBuffer& value)
    {
        uint16 size = value.getSize();
        uint16 size_little_endian = _BITSWAP16(size);
        _write<uint16>(size_little_endian);
        if( size > 0 )
        {
            _write(value.m_pBuff, size);
        }
        return *this;
    };
    IStreamBuffer& operator>>(IStreamBuffer& value)
    {
        uint16 size_little_endian = 0;
        _read<uint16>(size_little_endian);
        uint16 size = _BITSWAP16(size_little_endian);
        if( size > 0 )
        {
            value._resize(size + value.m_wPos);
            _read(&value.m_pBuff[value.m_wPos], size);
            value.m_wPos += size;
        }
        return *this;
    };

    IStreamBuffer& operator<<(const float& value);
    IStreamBuffer& operator>>(float &value);

    template<typename key_type, typename value_type>
    IStreamBuffer& operator << (const std::map<key_type, value_type>& data)
    {
        this->operator << ((uint32)(data.size()));
        for (auto it = data.begin();
            it != data.end(); ++it)
        {
            this->operator << (it->first);
            this->operator << (it->second);
        }
        return *this;
    };
    template<typename key_type, typename value_type>
    IStreamBuffer& operator >> (std::map<key_type, value_type>& data)
    {
        data.clear();
        uint32 data_size;
        this->operator >> (data_size);
        for (uint32 idx = 0; idx < data_size; ++idx)
        {
            key_type key_data;
            value_type value_data;
            this->operator >> (key_data);
            this->operator >> (value_data);
            data.insert(std::make_pair(key_data, value_data));
        }
        return *this;
    };


    //void PushUint32Array(const uint32* pArray, uint32 nCount);
    void put(size_t pos, const uint8 *src, size_t cnt) { _put(pos, src, cnt); };

    void put(size_t pos, const int8& value)  {	_put<int8>(pos,value ); }
    void put(size_t pos, const int16& value) {	_put<int16>(pos,_BITSWAP16(value) ); }
    void put(size_t pos, const int32& value) {	_put<int32>(pos,_BITSWAP32(value) ); }
    void put(size_t pos, const int64& value) {	_put<int64>(pos,_BITSWAP64(value) ); }
    void put(size_t pos, const uint8& value)  {	_put<uint8>(pos,value ); }
    void put(size_t pos, const uint16& value) {	_put<uint16>(pos,_BITSWAP16(value) ); }
    void put(size_t pos, const uint32& value) {	_put<uint32>(pos,_BITSWAP32(value) ); }
    void put(size_t pos, const uint64& value) {	_put<uint64>(pos,_BITSWAP64(value) ); }

protected:
    void _resize(size_t newsize)
    {
        if (newsize > m_buffSize)
        {
            __resize(newsize);
        }
        if (newsize > m_validateSize)
        {
            m_validateSize = newsize;
        }
    };
    virtual void _write(const uint8 *src, size_t size);
    virtual void _read(uint8 *dest, size_t size);
    virtual void _put(size_t pos,const uint8* src, size_t size);
    void _destroyBuff();

    template <typename T> void _write(const T& value)   { _write((uint8*)&value, sizeof(T)); }
    template <typename T> void _read(T &value)  { _read((uint8*)&value, sizeof(T)); }
    template <typename T> void _put(size_t pos,const T &value)   { _put(pos,(uint8*)&value,sizeof(T) ); }

protected:
    size_t m_buffSize;
    uint8 *m_pBuff;

    //private:
    size_t m_rPos;
    size_t m_wPos;
    size_t m_validateSize;
    IStreamBufferMode m_mode;
};


inline void IStreamBuffer::_write(const uint8 *src, size_t size)
{
    //assert((0 <= size) && (_wpos + size <= MAXSIZE));
    if ((m_mode & IStreamBufferWrite) == 0)
    {
        assert(m_mode & IStreamBufferWrite);
        return;
    }

    _resize(m_wPos + size);
    memcpy(&m_pBuff[m_wPos], src, size);
    m_wPos += size;
}

inline void IStreamBuffer::_read(uint8 *dest, size_t size)
{
    //assert((0 <= size) && (_rpos + size <= _validate_size));
    if ((m_mode & IStreamBufferRead) == 0)
    {
        assert(m_mode & IStreamBufferRead);
        return;
    }

    if (m_rPos + size <= m_validateSize)
    {
        memcpy(dest, &m_pBuff[m_rPos],  size);
        m_rPos += size;
    }
    else
    {
        // Note: Only can an INetPacket be read once
        // Call ResetPosRead if you really need read more than once
        THROW_EXCEPTION(BufferException());
    }
}

inline void IStreamBuffer::_put(size_t pos, const uint8* src, size_t size)
{
    //assert((0 <= size) && (pos + size <= MAXSIZE));

    _resize(pos + size);
    memcpy(&m_pBuff[pos], src, size);
}

inline IStreamBuffer& IStreamBuffer::operator<<(const char *value)
{
    size_t size = strlen(value);
    if(size > 1024*10)
    {
        assert(0);
        size = 1024*10;
    }
    uint16 size_little_endian = _BITSWAP16(size);
    _write<uint16>(size_little_endian);
    if ( size > 0)
    {
        _write((const uint8*)value, size);
    }
    return *this;
}

inline IStreamBuffer& IStreamBuffer::operator<<(const std::string &value)
{
    std::string utf8Value = value;
    uint16 size = utf8Value.size();
    uint16 size_little_endian = _BITSWAP16(size);
    _write<uint16>(size_little_endian);
    if ( size > 0)
    {
        _write((const uint8*)utf8Value.data(), size);
    }

    return *this;
}

inline IStreamBuffer& IStreamBuffer::operator>>(std::string &value)
{
    uint16 size_little_endian = 0;
    _read<uint16>(size_little_endian);
    uint16 size = _BITSWAP16(size_little_endian);
    value.clear();
    if ( size > 0 && size <= getRemainSize() )
    {
        value.clear();
        uint8* tmpBuf = new uint8[size];
        _read(tmpBuf, size);
        value.append( (char*)tmpBuf, size );
        delete [] tmpBuf;
    }
    return *this;
}

#ifdef WIN32
#define IS_NAN(x) (_isnan(x) != 0)
#else
#define IS_NAN(x) (isnan(x) != 0)
#endif

inline IStreamBuffer& IStreamBuffer::operator<<(const float& value) 
{
    if (IS_NAN(value))
    {
        THROW_EXCEPTION(BufferException());
    }
#if _LITTLE_ENDIAN_
    _write<float>(value);
#else
    uint32 tmp;
    tmp = *((uint32*)&value);
    //memcpy(&tmp, &value, 4);
    _write<uint32>(_BITSWAP32(tmp));
#endif
    return *this;
}

inline IStreamBuffer& IStreamBuffer::operator>>(float& value) 
{
#if _LITTLE_ENDIAN_
    _read<float>(value);
#else
    uint32 tmp = 0;
    _read<uint32>(tmp); 
    tmp = _BITSWAP32(tmp);
    //value = *((float*)&tmp);
    memcpy(&value, &tmp, 4);
#endif
    if (IS_NAN(value))
    {
        THROW_EXCEPTION(BufferException());
    }
    return *this; 
}

//////////////////////////////////////////////////////////////////////////

template <uint32 defaultSize>
class CBuffer : public IStreamBuffer
{
public:
    CBuffer() {};
    virtual ~CBuffer(){ __clear(); };
public:
    virtual void clear()
    {
        __clear();
        IStreamBuffer::clear();
    };

    CBuffer(const CBuffer& r)
    {
        clear();
        IStreamBuffer::operator =(r);
    };

    CBuffer& operator= (const CBuffer& r)
    {
        if (this != &r)
        {
            clear();
            IStreamBuffer::operator =(r);
        }
        return *this;
    };

private:
    virtual void __resize(size_t newsize)
    {
        if (m_buffSize == 0)
        {
            m_pBuff = &m_buffInternal[0];
            m_buffSize = sizeof(m_buffInternal);
        }
        if (newsize > m_buffSize)
        {
            newsize = (newsize > m_buffSize * 2) ? newsize : (m_buffSize * 2);
            uint8* new_buff = new uint8[newsize];
            if (m_pBuff && getSize() > 0)
            {
                memcpy(new_buff, m_pBuff, getSize());
            }
            if (m_pBuff && m_pBuff != &m_buffInternal[0])
                delete [] m_pBuff;
            m_pBuff = new_buff;
            m_buffSize = newsize;
        }
    };

    void __clear()
    {
        if (m_pBuff && m_pBuff != &m_buffInternal[0])
        {
            delete [] m_pBuff;
            m_pBuff = NULL;
            m_buffSize = 0;
        }
    };
protected:
    uint8 m_buffInternal[defaultSize];
};

typedef CBuffer<256> Buffer;

class FileBuffer :
    public CBuffer<4096>
{
public:
    FileBuffer(const char * szPath, const char * mode);
    ~FileBuffer();

    void flushFileBuffer();
    bool isOpened() const;
    void closeFileBuffer();
    void setBufferSize(int bufferSize);
    int getBufferSize() const;

protected:
    virtual void _write(const uint8 *src, size_t size);
    virtual void _read(uint8 *dest, size_t size);
    virtual void _put(size_t pos,const uint8* src, size_t size);

private:
    int             m_rFilePos;
    int             m_wFilePos;
    FILE           *m_pFile;
    std::string     m_mode;
    std::string     m_path;
};
