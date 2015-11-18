#include "Thread.h"




bool Mutex::Trylock()
{
	WaitForSingleObject( m_Mutex, 0 );
	return true;
}


int Mutex::Lock()
{
	if( WAIT_FAILED == WaitForSingleObject( m_Mutex, INFINITE ) )
		return -1;
	else 
		return 0;
}


int Mutex::Unlock()
{
	if( ReleaseMutex( m_Mutex ) )
		return 0;
	else 
		return -1;
}
