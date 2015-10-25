#include "Game.h"


bool Game::Init()
{
	if( !m_Gui.Init() )
		return false;

	return true;
}


void Game::Run()
{
	MSG _msg;
	ZeroMemory( &_msg, sizeof( _msg ) );

	static UINT uLastTime = GetTickCount();
	static UINT uTimeDelta;
	static UINT uCurTime;

	while( _msg.message != WM_QUIT )
	{
		if( PeekMessage( &_msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &_msg );
			DispatchMessage( &_msg );
		}
		else
		{
			uCurTime = GetTickCount();
			uTimeDelta = uCurTime - uLastTime;
			DoTick( uTimeDelta * 0.001f );
			uLastTime = uCurTime;
		}
	}
}


void Game::Release()
{
	
}


void Game::DoTick( float timeDelta )
{
	m_Gui.DoTick( timeDelta );
}

void Game::HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
}