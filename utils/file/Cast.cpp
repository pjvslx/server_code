#include <assert.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#include "Cast.h"
//#include "curl/curl.h"

#ifndef QUICK_COCOS2D
#pragma execution_character_set("utf-8")
#endif

wchar_t* Cstr2Wstr(const char *pcstr, wchar_t *pwstr)
{
    wchar_t* result = pwstr;
    if(pwstr != NULL && pcstr != NULL) {
        while(*pcstr != 0)
        {
            *pwstr = static_cast<wchar_t>(*pcstr);
            pwstr++;
            pcstr++;
        }
        *pwstr = L'\0';
        return result;
    }
    return 0;
}

std::wstring Cstr2Wstr(const char *pcstr)
{
    size_t length = strlen(pcstr);
    wchar_t* wstr = new wchar_t[length + 1];
    Cstr2Wstr(pcstr, wstr);
    std::wstring result = wstr;
    delete [] wstr;
    return result;
}


bool Utf82Unicode(const char* inStr, wchar_t* outStr, int buffLen)
{
    int inLen = (int)strlen(inStr);
    return Utf82Unicode(inStr, inLen, outStr, buffLen);
}

bool Utf82Unicode(const char* inStr, const int inLen, wchar_t* outStr, int& outLen)
{
    if(!inStr || !outStr)
    {
        return false;
    }

    int curLen = 0;
    assert(outLen > 1 && outLen > inLen);
    int maxLen = outLen - 1;

    int charIndex = 0;
    int unicodeIndex = 0;
    while(charIndex < inLen)
    {
        if (curLen + 1 >= maxLen)
        {
            assert(0);
            break;
        }
        //read one char.
        int charBytes = 0;
        const char* utf8 = &inStr[charIndex];
        wchar_t* unicodeChar = &outStr[unicodeIndex];
        *unicodeChar = (wchar_t)0;

        if( (utf8[0] & 0x80) == 0x0)
        {
            charBytes = 1;
            *unicodeChar = utf8[0];
        }
        else if( (utf8[0] & 0xE0) == 0xC0 )
        {
            charBytes = 2;
            *unicodeChar = (utf8[0] & 0x1F) << (6);
        }
        else if( (utf8[0] & 0xF0) == 0xE0 )
        {
            charBytes = 3;
            *unicodeChar = (utf8[0] & 0x0F) << (12);
        } 
#if 0
        else if( (utf8[0] & 0xF8) == 0xF0 )
        {
            charBytes = 4;
            *unicodeChar = (utf8[0] & 0x07) << (18);
        }
        else if( (utf8[0] & 0xFC) == 0xF8 )
        {
            charBytes = 5;
            *unicodeChar = (utf8[0] & 0x03) << (24);
        }
        else if( (utf8[0] & 0xFE) == 0xFC )
        {
            charBytes = 6;
            *unicodeChar = (utf8[0] & 0x01) << (30);
        }
#endif
        for(int c = 1; c < charBytes; c++)
        {
            *unicodeChar |= (utf8[c] & 0x3F) << (charBytes - c - 1) * 6;
        }

        if(charBytes == 0)
        {
            charIndex ++;
        }
        else
        {
            charIndex += charBytes;
            unicodeIndex++;
            curLen++;
        }
    }

    outLen = unicodeIndex;
    outStr[outLen] = 0;

    return true;
}

bool Unicode2Utf8(const wchar_t* inStr, char* outStr, int buffLen)
{
    int inLen = (int)wcslen(inStr);
    return Unicode2Utf8(inStr, inLen, outStr, buffLen);
}

bool Unicode2Utf8(const wchar_t* inStr, const int inLen, char* outStr, int& outLen)
{
    if(!inStr || !outStr) {
        return false;
    }

    int curLen = 0;
    int maxLen = inLen * 6;
    if (outLen > 0) 
        maxLen = outLen - 1;

    int unicodeIndex = 0;
    char * outChar = outStr;
    while(unicodeIndex < inLen)
    {
        wchar_t curUnicodeChar = *(inStr + unicodeIndex);

        if (curLen + 1 >= maxLen)
        {
            assert(0);
            break;
        }
        int charBytes = 0;
        if(curUnicodeChar < 0x7F)
        {
            charBytes = 1;
            *outChar++= curUnicodeChar & 0x7F;
        }
        else if(curUnicodeChar < 0x7FF)
        {
            charBytes = 2;
            *outChar++=0xC0 | (curUnicodeChar >> 6);
        }
        else if(curUnicodeChar < 0xFFFF)
        {
            charBytes = 3;
            *outChar++=0xE0 | (curUnicodeChar >> 12);
        }
#if 0
        else if(curUnicodeChar < 0x1FFFFF)
        {
            charBytes = 4;
            *outChar++=0xF0 | (curUnicodeChar >> 18);
        }
        else if(curUnicodeChar < 0x3FFFFFF)
        {
            charBytes = 5;
            *outChar++=0xF8 | (curUnicodeChar >> 24);
        }
        else if(curUnicodeChar < 0x7FFFFFFF)
        {
            charBytes = 6;
            *outChar++= 0xFC | (curUnicodeChar >> 30);
        }
#endif

        if (curLen + charBytes >= maxLen)
        {
            outChar--;
            assert(0);
            break;
        }
        curLen += charBytes;
        for(int c = 1; c < charBytes; c++)
        {
            *outChar++ = 0x80 | ((curUnicodeChar >> (charBytes - c - 1) * 6) & 0x3F);
        }

        unicodeIndex ++;

    }
    *outChar = 0;
    outLen = (int)(outChar - outStr);


    return true;
}

int CountStringCN2EU1_utf8(const char* inStr)
{
    int count = 0;
    int charIndex = 0;
    while(inStr[charIndex]) {
        //read one char.
        int charBytes = 0;
        const char utf8_char = inStr[charIndex];

        if( (utf8_char & 0x80) == 0x0)
        {
            charBytes = 1;
        }
        else if( (utf8_char & 0xE0) == 0xC0 )
        {
            charBytes = 2;
        }
        else if( (utf8_char & 0xF0) == 0xE0 )
        {
            charBytes = 3;
        } 
#if 0
        else if( (utf8_char & 0xF8) == 0xF0 )
        {
            charBytes = 4;
        }
        else if( (utf8_char & 0xFC) == 0xF8 )
        {
            charBytes = 5;
        }
        else if( (utf8_char & 0xFE) == 0xFC )
        {
            charBytes = 6;
        }
#endif
        count += charBytes >=3 ? 2 : 1;
        if(charBytes == 0) {
            charIndex ++;
        } else {
            charIndex += charBytes;
        }
    }
    return count;
}


int CountStringCN2EU1_unicode(const wchar_t* inStr)
{
    int count = 0;
    int unicodeIndex = 0;
    for( ;inStr[unicodeIndex]; unicodeIndex ++)
    {
        wchar_t curUnicodeChar = inStr[unicodeIndex];

        int charBytes = 0;
        if(curUnicodeChar < 0x7F)
        {
            charBytes = 1;
        }
        else if(curUnicodeChar < 0x7FF)
        {
            charBytes = 2;
        }
        else if(curUnicodeChar < 0xFFFF)
        {
            charBytes = 3;
        }
#if 0
        else if(curUnicodeChar < 0x1FFFFF)
        {
            charBytes = 4;
        }
        else if(curUnicodeChar < 0x3FFFFFF)
        {
            charBytes = 5;
        }
        else if(curUnicodeChar < 0x7FFFFFFF)
        {
            charBytes = 6;
        }
#endif
        count += charBytes >=3 ? 2 : 1;


    }
    return count;
}

bool LimitStringCN2EU1_utf8(char *inStr, int limitLen)
{
    int count = 0;
    int charIndex = 0;
    while(inStr[charIndex]) {
        //read one char.
        int charBytes = 0;
        const char utf8_char = inStr[charIndex];

        if( (utf8_char & 0x80) == 0x0)
        {
            charBytes = 1;
        }
        else if( (utf8_char & 0xE0) == 0xC0 )
        {
            charBytes = 2;
        }
        else if( (utf8_char & 0xF0) == 0xE0 )
        {
            charBytes = 3;
        } 
#if 0
        else if( (utf8_char & 0xF8) == 0xF0 )
        {
            charBytes = 4;
        }
        else if( (utf8_char & 0xFC) == 0xF8 )
        {
            charBytes = 5;
        }
        else if( (utf8_char & 0xFE) == 0xFC )
        {
            charBytes = 6;
        }
#endif
        count += charBytes >=3 ? 2 : 1;
        if(count > limitLen)
        {
            inStr[charIndex] = 0;
            return true;
        }

        if(charBytes == 0)
        {
            charIndex++;
        }
        else
        {
            charIndex += charBytes;
        }
    }
    return false;
}

bool LimitStringCN2EU1_unicode(wchar_t *inStr, int limitLen)
{
    int count = 0;
    int unicodeIndex = 0;
    for( ;inStr[unicodeIndex]; unicodeIndex ++)
    {
        wchar_t curUnicodeChar = inStr[unicodeIndex];

        int charBytes = 0;
        if(curUnicodeChar < 0x7F)
        {
            charBytes = 1;
        }
        else if(curUnicodeChar < 0x7FF)
        {
            charBytes = 2;
        }
        else if(curUnicodeChar < 0xFFFF)
        {
            charBytes = 3;
        }
#if 0
        else if(curUnicodeChar < 0x1FFFFF)
        {
            charBytes = 4;
        }
        else if(curUnicodeChar < 0x3FFFFFF)
        {
            charBytes = 5;
        }
        else if(curUnicodeChar < 0x7FFFFFFF)
        {
            charBytes = 6;
        }
#endif
        count += charBytes >=3 ? 2 : 1;
        if(count > limitLen)
        {
            inStr[unicodeIndex] = 0;
            return true;
        }
    }
    return false;

}

wchar_t *s_dbBufferUTF16 = NULL;
char *s_dbBufferUTF8 = NULL;

const char * FilterString(const char *inStr, int &outLen, bool isFrench) //Special function to convert string in database for auto_line_feed and non-break-spaces
{
    if(s_dbBufferUTF16 == NULL)
    {
        s_dbBufferUTF16 = new wchar_t[20000];
        s_dbBufferUTF8 = new char[60000];
    }
    Utf82Unicode(inStr, s_dbBufferUTF16, 20000);
#define HTML_TAG_START 0x12
#define HTML_TAG_END   0x13
    //int k=0;
    int inLen = 0;
    for(int i=0; s_dbBufferUTF16[i] != 0; i++)
    {
        if(s_dbBufferUTF16[i] == L'~')
        {
            s_dbBufferUTF16[i] = 17;
        }
        else if(s_dbBufferUTF16[i] == L'^')
        {
            s_dbBufferUTF16[i]= 0xA0;
        }
        else if(s_dbBufferUTF16[i] == L'<' || s_dbBufferUTF16[i] == L'[')
        {
            s_dbBufferUTF16[i]= HTML_TAG_START;
        }
        else if(s_dbBufferUTF16[i] == L'>' || s_dbBufferUTF16[i] == L']')
        {
            s_dbBufferUTF16[i]= HTML_TAG_END;
        }
        else if(isFrench) //Remove Accents
        {

            if(s_dbBufferUTF16[i] ==0x00C0)
                s_dbBufferUTF16[i] = L'A';      // A`       0xC0 ->A
            else if(s_dbBufferUTF16[i] ==0x00C2)
                s_dbBufferUTF16[i] = L'A';      // A^       0xC2 ->A
            else if(s_dbBufferUTF16[i] ==0x00C7)
                s_dbBufferUTF16[i] = L'C';      // C        0xC7 ->C
            else if(s_dbBufferUTF16[i] ==0x00C8)
                s_dbBufferUTF16[i] = L'E';      // E        0xC8 ->E
            else if(s_dbBufferUTF16[i] ==0x00C9)
                s_dbBufferUTF16[i] = L'E';      // E'       0xC9 ->E
            else if(s_dbBufferUTF16[i] ==0x00CA)
                s_dbBufferUTF16[i] = L'E';      // E^       0xCA ->E
            else if(s_dbBufferUTF16[i] ==0x00CB)
                s_dbBufferUTF16[i] = L'E';      // E:       0xCB ->E
            else if(s_dbBufferUTF16[i] ==0x00CE)
                s_dbBufferUTF16[i] = L'I';      // I^       0xCE ->I
            else if(s_dbBufferUTF16[i] ==0x00CF)
                s_dbBufferUTF16[i] = L'I';      // I:       0xCF ->I
            else if(s_dbBufferUTF16[i] ==0x00D4)
                s_dbBufferUTF16[i] = L'O';      // O^       0xD4 ->O
            else if(s_dbBufferUTF16[i] ==0x00D9)
                s_dbBufferUTF16[i] = L'U';      // U`       0xD9 ->O
            else if(s_dbBufferUTF16[i] ==0x00DB)
                s_dbBufferUTF16[i] = L'U';      // U^       0xDB ->O

        }
        inLen++;
    }
#undef HTML_TAG_START
#undef HTML_TAG_END
    inLen++;
    outLen = 0;
    Unicode2Utf8(s_dbBufferUTF16, inLen, s_dbBufferUTF8, outLen);
    return s_dbBufferUTF8;
}
#if defined(_WIN32) || defined(_WIN64)

int ConvertEncoding(UINT nSourceCP, const char * lpszText, UINT nDestCP, char * lpszOutput, int cbOutput)
{
    DWORD cchUnicodeText = MultiByteToWideChar(nSourceCP, 0, lpszText, -1, NULL, 0);
    LPWSTR lpszUnicodeText = (LPWSTR)malloc((cchUnicodeText + 1) * sizeof(WCHAR));
    MultiByteToWideChar(nSourceCP, 0, lpszText, -1, lpszUnicodeText, cchUnicodeText + 1);

    if (NULL == lpszOutput && 0 == cbOutput)
    {
        DWORD cbDestText = WideCharToMultiByte(nDestCP, 0, lpszUnicodeText, -1, NULL, 0, NULL, NULL);
        free(lpszUnicodeText);
        return cbDestText;
    }
    else
    {
        DWORD cbConverted = WideCharToMultiByte(nDestCP, 0, lpszUnicodeText, -1, lpszOutput, cbOutput, NULL, NULL);
        free(lpszUnicodeText);
        return cbConverted;
    }
}

int AnsiToUTF8(const char * lpszText, char * lpszOutput, int cbOutput)
{
    return ConvertEncoding(CP_ACP, lpszText, CP_UTF8, lpszOutput, cbOutput);
}

int UTF8ToAnsi(const char * lpszText, char * lpszOutput, int cbOutput)
{
    return ConvertEncoding(CP_UTF8, lpszText, CP_ACP, lpszOutput, cbOutput);
}

std::string ConvertEncoding(UINT nSourceCP, const std::string& text, UINT nDestCP)
{
    int cbOutput = ConvertEncoding(nSourceCP, text.c_str(), nDestCP, NULL, 0);
    char * lpszOutput = (char *)malloc(cbOutput + 1);
    ConvertEncoding(nSourceCP, text.c_str(), nDestCP, lpszOutput, cbOutput + 1);
    std::string destText = lpszOutput;
    free(lpszOutput);
    return destText;
}

std::string UnicodeToAnsi(const std::wstring& text, UINT nCodePage)
{
    DWORD cbAnsiText = WideCharToMultiByte(nCodePage, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
    LPSTR lpszAnsiText = (LPSTR)malloc((cbAnsiText + 1) * sizeof(CHAR));
    WideCharToMultiByte(nCodePage, 0, text.c_str(), -1, lpszAnsiText, cbAnsiText + 1, NULL, NULL);
    std::string ansiText = lpszAnsiText;
    free(lpszAnsiText);
    return ansiText;
}

std::wstring AnsiToUnicode(const std::string& text, UINT nCodePage)
{
    DWORD cchUnicodeText = MultiByteToWideChar(nCodePage, 0, text.c_str(), -1, NULL, 0);
    LPWSTR lpszUnicodeText = (LPWSTR)malloc((cchUnicodeText + 1) * sizeof(WCHAR));
    MultiByteToWideChar(nCodePage, 0, text.c_str(), -1, lpszUnicodeText, cchUnicodeText + 1);
    std::wstring unicodeText = lpszUnicodeText;
    free(lpszUnicodeText);
    return unicodeText;
}

std::string AnsiToUTF8(const std::string& text)
{
    int cbOutput = ConvertEncoding(CP_ACP, text.c_str(), CP_UTF8, NULL, 0);
    char * lpszOutput = (char *)malloc(cbOutput + 1);
    ConvertEncoding(CP_ACP, text.c_str(), CP_UTF8, lpszOutput, cbOutput + 1);
    std::string destText = lpszOutput;
    free(lpszOutput);
    return destText;
}

std::string UTF8ToAnsi(const std::string& text)
{
    int cbOutput = ConvertEncoding(CP_UTF8, text.c_str(), CP_ACP, NULL, 0);
    char * lpszOutput = (char *)malloc(cbOutput + 1);
    ConvertEncoding(CP_UTF8, text.c_str(), CP_ACP, lpszOutput, cbOutput + 1);
    std::string destText = lpszOutput;
    free(lpszOutput);
    return destText;
}

#endif

std::vector<int> FindMetaNotes(int max)
{
    std::vector<int> metas;
    int meta[3] = {1, 2, 5};
    int base = 1;
    while(base <= max)
    {
        for (int i = 0; i < 3; ++i)
        {
            int metaNote = base * meta[i];
            if (metaNote <= max)
            {
                metas.push_back(metaNote);
            }
            else
            {
                break;
            }
        }
        base *= 10;
    }
    return metas;
}

std::vector<int> CurrencyDecomposition( int number, int maxUnit )
{
    std::vector<int> notes;
    std::vector<int> metas = FindMetaNotes(maxUnit);
    int remaining = number;
    for(int i = (int)metas.size() - 1; i >= 0; --i)
    {
        int note = metas[i];
        while(remaining >= note)
        {
            notes.push_back(note);
            remaining -= note;
        }
    }
    std::reverse(notes.begin(), notes.end());
    return notes;
}


std::string std::format( const char * fmt, ... )
{
    std::string result;
    
    va_list	args;
    va_start(args, fmt);
#ifdef IOS
    // https://gist.github.com/foobit/4618064
    char * szText = NULL;
    int len = vsnprintf(NULL, 0, fmt, args);
    if (len > 0)
    {
        szText = (char *)malloc((len + 1) * sizeof(char));
        
        // va_end/va_start hack to fix *nix 64bit versions inc Mac OS X
        va_end(args);
        va_start(args, fmt);
        vsprintf(szText, fmt, args);
        //vsnprintf(szText, len, fmt, args);
        result = szText;
        free(szText);
    }
#else
    result = std::formatv(fmt, args);
#endif
    va_end(args);
    return result;
}

std::string std::formatv( const char * fmt, va_list args )
{
    char * szText = NULL;
#if (defined IOS || defined ANDROID)
    size_t length = vsnprintf(NULL, 0, fmt, args);
#else
    size_t length = _vscprintf(fmt, args);
#endif
    szText = (char *)malloc(length + 1);
    vsprintf(szText, fmt, args);
    std::string text = szText;
    free(szText);
    return text;
}

#if (!defined IOS && !defined ANDROID)

std::wstring std::format( const wchar_t * format, ... )
{
    wchar_t * szText = NULL;
    va_list args;
    va_start(args, format);
    size_t length = _scwprintf(format, args);
    va_start(args, format);
    szText = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
    wsprintfW(szText, format, args);
    va_end(args);
    std::wstring text = szText;
    free(szText);
    return text;
}

#endif
