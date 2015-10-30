#ifndef __H_STRINGCVTOOLS_H__
#define __H_STRINGCVTOOLS_H__
#include <string>


extern const std::wstring	MultibyteToUnicode( const char* szSrc );

extern const std::string	UnicodeToMultibyte( const wchar_t* szSrc );

extern const std::string	UnicodeToUTF8( const wchar_t* szSrc );

extern const std::wstring	UTF8ToUnicode( const char* szSrc );


#if defined( _UNICODE ) || defined( UNICODE )

#define UnicodeToANSI UnicodeToMultibyte
#define ANSIToUnicode



#else

#define ANSIToUnicode MultibyteToUnicode
#define UnicodeToANSI

#endif 






#endif // __H_STRINGCVTOOLS_H__