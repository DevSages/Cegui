#ifndef __H_WINDOW_H__
#define __H_WINDOW_H__
#include <set>
#include <vector>
#include <assert.h>
#include <Windows.h>


class Input
{
public:
	virtual void			HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;
};


class GameWnd
{
public:
	static GameWnd&			GetWnd();

	void					AddInputHander( Input* pInputHandler );

	bool					InitWnd( UINT uWidth, UINT uHeight, bool bFullScreen = false );

	void					ShowWindow();

	void					HideWindow();
	
	void					DestroyWnd();
 
private:
	static LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	GameWnd():m_Hwnd( NULL ),m_Hinst( NULL ),m_Width( 0 ),m_Height( 0 ),m_bFullScreen( false )
	{
		
	}

	GameWnd( const GameWnd& r )
	{

	}

public:
	HWND					m_Hwnd;
	HINSTANCE				m_Hinst;
	UINT					m_Width;
	UINT					m_Height;
	bool					m_bFullScreen;

protected:
	typedef std::set< Input* > Input_Handelers;
	typedef Input_Handelers::iterator Input_HandlersIter;
	static Input_Handelers	s_InputHandlers;
};

#endif // __H_WINDOW_H__