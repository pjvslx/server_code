#pragma once

#ifdef IOS
#include <sys/_types/_int64_t.h>
#include <sys/_types/_int32_t.h>
#include <sys/_types/_int16_t.h>
#include <sys/_types/_int8_t.h>
#include <_types/_uint64_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint16_t.h>
#include <_types/_uint8_t.h>

typedef  uint8_t	BYTE;
typedef  uint32_t	UINT;
#else
typedef  unsigned char	BYTE;
typedef  unsigned int	UINT;
#endif

#ifdef __GNUC__
#ifdef IOS
// Make same for 32/64 bit
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef uint32_t DWORD;
typedef uint8_t byte;
#else
typedef long long int64;
typedef int int32;
typedef short int16;
typedef signed char int8;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned int DWORD;
typedef unsigned char byte;
#endif
#else
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#endif

#ifdef __GNUC__
#define DEPRECATED(func)	func __attribute__ ((deprecated))
#ifdef IOS
#define FORMAT_FUNC(nf,np)
#else
#define FORMAT_FUNC(nf,np)	__attribute__((format(printf,nf,np)))
#endif
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#define FORMAT_FUNC(nf,np)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#define FORMAT_FUNC(nf,np)
#endif

typedef uint32 ObjGUID;
const static ObjGUID ObjGUID_NULL = 0;

union InstanceGUID
{
    inline bool operator<( const InstanceGUID& p ) const
    {
        return instanceGUID < p.instanceGUID;
    }

    inline bool operator==(const InstanceGUID &other) const
    {
        return instanceGUID == other.instanceGUID;
    }

    inline bool operator!=(const InstanceGUID &other) const
    {
        return instanceGUID != other.instanceGUID;
    }

    uint64 instanceGUID;
    struct 
    {
        uint16 GSID;
        uint16 MAPID;
        uint32 UID;
    };
};

const static InstanceGUID InsGUID_INVALID = {0};



#if defined(__amd64__)||defined(__amd64)||defined(__x86_64__)||defined(__x86_64)||defined(_M_X64)
typedef uint64 dword_ptr;
#else
typedef uint32 dword_ptr;
#endif 


typedef	uint32				U32;
typedef	uint16				U16;
typedef	uint8				U8;
typedef	int32				S32;
typedef	int16				S16;
typedef	int8				S8;
typedef float				F32;

#ifdef __GNUC__

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#else

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define atoll _atoi64
#define ltoa  _ltoa
#endif

#ifdef __GNUC__
#	define LIKELY( _x ) \
    __builtin_expect( ( _x ), 1 )
#	define UNLIKELY( _x ) \
    __builtin_expect( ( _x ), 0 )
#else
#	define LIKELY( _x ) \
    _x
#	define UNLIKELY( _x ) \
    _x
#endif

#if 0
// Will use for refractoring in future, do NOT delete
template <typename T>
struct Nullable {

    Nullable()
        : _hasValue(false)
    {
    };

    Nullable(T value)
        : _value(value)
        , _hasValue(true)
    {
    };

    bool hasValue() const 
    { 
        return _hasValue; 
    };
    T& value() 
    { 
        assert(_hasValue);
        return  _value; 
    };

    const T& value() const
    { 
        assert(_hasValue);
        return  _value; 
    };

    operator T&()
    {
        return value();
    };

    T& operator->() 
    { 
        assert(_hasValue);
        return _value; 
    };

    Nullable<T>& operator=(const T& value)
    {
        _value = value;
        _hasValue = true;
    };

private:
    T       _value;
    bool    _hasValue;

};

struct tbool : public Nullable<bool>
{
public:
    tbool()
        : Nullable()
    {
    };

    tbool(bool value)
        : Nullable(value)
    {
    };

    bool isTrue() const 	
    {
        return this->hasValue() && this->value();
    };

    bool isFalse() const 	
    {
        return this->hasValue() && !this->value();
    };

    bool isSet() const 	
    {
        return this->hasValue();
    };

};
#endif



// CPU endian
#ifndef _BIG_ENDIAN_
#define _LITTLE_ENDIAN_ 1
#endif

#if !_LITTLE_ENDIAN_
#define _BITSWAP16(x) (((x << 8) & 0xff00) | ((x >> 8) & 0x00ff))

#define _BITSWAP32(x) (((x << 24) & 0xff000000) | ((x <<  8) & 0x00ff0000) | \
    ((x >>  8) & 0x0000ff00) | ((x >> 24) & 0x000000ff))

#define _BITSWAP64(x) (((x << 56) & 0xff00000000000000ULL) | ((x << 40) & 0x00ff000000000000ULL) | \
    ((x << 24) & 0x0000ff0000000000ULL) | ((x <<  8) & 0x000000ff00000000ULL) | \
    ((x >>  8) & 0x00000000ff000000ULL) | ((x >> 24) & 0x0000000000ff0000ULL) | \
    ((x >> 40) & 0x000000000000ff00ULL) | ((x >> 56) & 0x00000000000000ffULL))
#else
#define _BITSWAP16(x) (x)
#define _BITSWAP32(x) (x)
#define _BITSWAP64(x) (x)
#endif

struct FTableHeader
{
    uint8       FLAG[8]; 
    uint32      nVersion;
    uint32      nRows;
    uint32      nCols;		
    uint32      nOffsetIndex;
    uint32      nOffsetFormat;
    uint32      nOffsetEntry;
    uint32      nEntrySize;
    uint32      nOffsetStrTable;
    uint32      nLengthStrTable;
};
#define	TABLE_STR_FLAG      "FTABLE"
#define TABLE_VERSION       0x100000

