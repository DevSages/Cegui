#include "FileHandler.h"




FileHandler::FileHandler():
	m_pFile( NULL )
{

}

FileHandler::~FileHandler()
{
	Close();
}


bool FileHandler::OpenFile( const char* szFileName, const char* szOpenFlag )
{
	if( szFileName == NULL )
		RETURN_FALSE;
	errno_t errorCode = fopen_s( &m_pFile, szFileName, szOpenFlag );
	if( m_pFile == NULL || errorCode != 0 )
		RETURN_FALSE;

	RETURN_TRUE;
}

void FileHandler::Close()
{
	if( m_pFile != NULL )
	{
		fclose( m_pFile );
	}

	m_pFile = NULL;
}

void FileHandler::WriteFile( const char* szStrem, size_t size )
{
	if( m_pFile == NULL || szStrem == NULL || size == 0 )
		RETURN_EMPTY;
	size_t _size = fwrite( szStrem, size, 1, m_pFile );
	if( size != _size )
		RETURN_EMPTY;
	fflush( m_pFile );
}

long FileHandler::GetFileSize()
{
	if( m_pFile == NULL )
		RETURN_FALSE;

	MoveEnd();
	long size = ftell( m_pFile );
	MoveBegin();
	return size;
}

void FileHandler::MoveBegin()
{
	fseek( m_pFile, 0, SEEK_SET );
}

void FileHandler::MoveEnd()
{
	fseek( m_pFile, 0, SEEK_END );
}

bool FileHandler::GetLineContent( LINES& vContents )
{
	size_t fileSize = 0;
	if( !ValidFileSize( fileSize ) )
		RETURN_FALSE;

	if( !vContents.empty() )
	{
		vContents.clear();
	}

	char szLineBuf[COMMON_CHAR_BUF_128];
	while( !IsEndOfFile() )
	{
		char* szContent = NULL;
		std::string strLine;
		do
		{
			szContent = fgets( szLineBuf, COMMON_CHAR_BUF_128, m_pFile );
			if( szContent != NULL )
			{
				strLine += szContent;
			}
		}while( !IsNextLine( szLineBuf[strlen(szLineBuf)-1] ) && !IsEndOfFile() && szContent != NULL );

		vContents.push_back( strLine );
	}

	if( vContents.empty() )
		RETURN_FALSE;

	RETURN_TRUE;
}

string FileHandler::GetLineContent( size_t uLineIndex  )
{
	size_t lineIndex = 0;
	char szLineBuf[COMMON_CHAR_BUF_128];
	while( !IsEndOfFile() )
	{
		char* szContent = NULL;
		std::string strLine;
		do
		{
			szContent = fgets( szLineBuf, COMMON_CHAR_BUF_128, m_pFile );
			if( szContent != NULL )
			{
				strLine += szContent;
			}
		}while( !IsNextLine( szLineBuf[strlen(szLineBuf)-1] ) && !IsEndOfFile() && szContent != NULL );

		if( lineIndex == uLineIndex )
			return string( strLine );
		lineIndex++;
	}
	return string("");
}

bool FileHandler::GetContent( string& content )
{
	size_t fileSize = 0;
	if( !ValidFileSize( fileSize ) )
		RETURN_FALSE;

	char* szStream = new char[fileSize];
	fread( szStream, fileSize, 1, m_pFile );
	content = szStream;
	SAFE_DELETE( szStream );

	size_t strSize = content.size();
	if( strSize < fileSize )
	{
		content.clear();
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

bool FileHandler::ValidFileSize( size_t& uFileSize )
{
	long fileSize = GetFileSize();
	if( fileSize == -1 || fileSize == 0 )
	{
		uFileSize = 0;
		RETURN_FALSE;
	}
	uFileSize = fileSize;
	RETURN_TRUE;
}


bool FileHandler::IsNextLine( const char c )
{
	return ( c == '\n' || c == '\r' );
}

bool FileHandler::IsEndOfFile()
{
	if( m_pFile != NULL )
	{
		return ( feof(m_pFile) != ZERO || ferror(m_pFile) != ZERO );
	}
	return true;
}