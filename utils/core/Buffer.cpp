#include "Buffer.h"

#ifndef MIN
#define MIN(a, b)                   ((a) < (b) ? (a) : (b))
#endif

void FileBuffer::setBufferSize(int bufferSize)
{
    if (bufferSize > m_buffSize)
    {
        _resize(bufferSize);
    }
}

int FileBuffer::getBufferSize() const
{
    return (int)m_buffSize;
}

FileBuffer::FileBuffer( const char * szPath, const char * mode )
    : m_path(szPath)
    , m_mode(mode)
    , m_pFile(fopen(szPath, mode))
    , m_rFilePos(0)
    , m_wFilePos(0)
{
    _resize(4096);
}

FileBuffer::~FileBuffer()
{
    closeFileBuffer();
}

bool FileBuffer::isOpened() const
{
    return m_pFile != NULL;
}

void FileBuffer::closeFileBuffer()
{
    if (m_pFile != NULL)
    {
        flushFileBuffer();
        fclose(m_pFile);
        m_pFile = NULL;
    }
}


inline void FileBuffer::_write(const uint8 *src, size_t size)
{
    //assert((0 <= size) && (_wpos + size <= MAXSIZE));
    size_t bytesToCopy = size;
    size_t bytesCopied = 0;
    while(bytesToCopy > 0)
    {
        size_t copy_size = MIN(m_buffSize - m_wPos, bytesToCopy);
        memcpy(&m_pBuff[m_wPos], src + bytesCopied, copy_size);
        bytesCopied += copy_size;
        bytesToCopy -= copy_size;
        m_wPos += copy_size;
        m_wFilePos += copy_size;
        if (m_wPos == m_buffSize)
        {
            fwrite(m_pBuff, sizeof(uint8), m_wPos,  m_pFile);
            m_wPos = 0;
        }
    }

}

inline void FileBuffer::_read(uint8 *dest, size_t size)
{
    //assert((0 <= size) && (_rpos + size <= _validate_size));
    size_t bytesToRaed = size;
    size_t bytesRead = 0;
    while (bytesToRaed > 0)
    {
        if (m_rFilePos == ftell(m_pFile))
        {
            m_validateSize = fread(m_pBuff, sizeof(uint8), m_buffSize, m_pFile);
            if (0 == m_validateSize)
            {
                THROW_EXCEPTION(BufferException());
            }
            m_rPos = 0;
        }
        size_t copy_size = MIN(m_validateSize - m_rPos, bytesToRaed);
        memcpy(dest + bytesRead, &m_pBuff[m_rPos],  copy_size);
        m_rPos += copy_size;
        m_rFilePos += copy_size;
        bytesRead += copy_size;
        bytesToRaed -= copy_size;
    }
}

inline void FileBuffer::_put(size_t pos, const uint8* src, size_t size)
{
    //assert((0 <= size) && (pos + size <= MAXSIZE));
    flushFileBuffer();
    fseek(m_pFile, pos, SEEK_SET);
    m_wFilePos = (int)pos;
    m_wPos = 0;
    _write(src, size);
}

void FileBuffer::flushFileBuffer()
{
    if (m_pFile != NULL && m_wPos != 0)
    {
        fwrite(m_pBuff, sizeof(uint8), m_wPos,  m_pFile);
        fflush(m_pFile);
        m_wPos = 0;
    }
}

#undef MIN