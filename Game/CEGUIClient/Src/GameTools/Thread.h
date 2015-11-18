#ifndef __H_THREAD_H__
#define __H_THREAD_H__
#include <Windows.h>




class Mutex
{
public:
	Mutex()
	{
		m_Mutex = CreateMutex(NULL, false, NULL);
	}
	~Mutex()
	{
		CloseHandle( m_Mutex);
	}
public:
	int				Lock();

	int				Unlock();

	bool			Trylock();

private:
	HANDLE          m_Mutex;
};





#endif // __H_THREAD_H__