#include "StringCVTools.h"
#include "ErrorMsg.h"
#include "Common.h"
#include <Windows.h>




const std::wstring MultibyteToUnicode( const char* szSrc )
{  
	int length = MultiByteToWideChar( CP_ACP, 0, szSrc, -1, NULL, 0 );
	if( length <= 0 )
	{
		ErrorMsg( "MultibyteToUnicode() Failed, ErrorCode = %d!", GetLastError() );
		return std::wstring(L"");
	}

	wchar_t* pUnicode = new wchar_t[length+1];
	if( pUnicode == NULL )
	{
		ErrorMsg( "MultibyteToUnicode() pUnicode = NULL" );
		return std::wstring(L"");
	}

	if( MultiByteToWideChar( CP_ACP, 0, szSrc, -1, pUnicode, length ) != 0 )
	{
		std::wstring rt = std::wstring( pUnicode );
		SAFE_DELETE( pUnicode );
		return rt;
	}

	ErrorMsg( "MultibyteToUnicode() Failed, ErrorCode = %d!", GetLastError() );
	return std::wstring(L"");
}


const std::string UnicodeToMultibyte( const wchar_t* szSrc )
{
	int length = WideCharToMultiByte( CP_ACP, 0, szSrc, -1, NULL, 0, NULL, NULL );
	if( length <= 0 )
	{
		ErrorMsg( "UnicodeToMultibyte() Failed, ErrorCode = %d!", GetLastError() );
		return std::string("");
	}

	char* pMultibyte = new char[length + 1];
	if( pMultibyte == NULL )
	{
		ErrorMsg( "MultibyteToUnicode() pMultibyte = NULL" );
		return std::string("");
	}

	if( WideCharToMultiByte( CP_ACP, 0, szSrc, -1, pMultibyte, length, NULL, NULL ) != 0 )
	{
		std::string rt = std::string( pMultibyte );
		SAFE_DELETE( pMultibyte );
		return rt;
	}

	ErrorMsg( "UnicodeToMultibyte() Failed, ErrorCode = %d!", GetLastError() );
	return std::string("");
}


const std::string UnicodeToUTF8( const wchar_t* szSrc )
{
	int length = WideCharToMultiByte( CP_UTF8, 0, szSrc, -1, NULL, 0, NULL, NULL );
	if( length <= 0 )
	{
		ErrorMsg( "UnicodeToMultibyte() Failed, ErrorCode = %d!", GetLastError() );
		return std::string("");
	}

	char* pMultibyte = new char[length + 1];
	if( pMultibyte == NULL )
	{
		ErrorMsg( "MultibyteToUnicode() pMultibyte = NULL" );
		return std::string("");
	}

	if( WideCharToMultiByte( CP_UTF8, 0, szSrc, -1, pMultibyte, length, NULL, NULL ) != 0 )
	{
		std::string rt = std::string( pMultibyte );
		SAFE_DELETE( pMultibyte );
		return rt;
	}

	ErrorMsg( "UnicodeToMultibyte() Failed, ErrorCode = %d!", GetLastError() );
	return std::string("");
}


const std::wstring UTF8ToUnicode( const char* szSrc )
{
	int length = MultiByteToWideChar( CP_UTF8, 0, szSrc, -1, NULL, 0 );
	if( length <= 0 )
	{
		ErrorMsg( "MultibyteToUnicode() Failed, ErrorCode = %d!", GetLastError() );
		return std::wstring(L"");
	}

	wchar_t* pUnicode = new wchar_t[length+1];
	if( pUnicode == NULL )
	{
		ErrorMsg( "MultibyteToUnicode() pUnicode = NULL" );
		return std::wstring(L"");
	}

	if( MultiByteToWideChar( CP_UTF8, 0, szSrc, -1, pUnicode, length ) != 0 )
	{
		std::wstring rt = std::wstring( pUnicode );
		SAFE_DELETE( pUnicode );
		return rt;
	}

	ErrorMsg( "MultibyteToUnicode() Failed, ErrorCode = %d!", GetLastError() );
	return std::wstring(L"");
}