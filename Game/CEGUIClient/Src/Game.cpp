#include "Game.h"


Game theGame;



bool Game::Init()
{
	if( !m_Gui.Init() )
		return false;

	if( !m_Logic.Init() )
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
	m_Gui.Release();
	m_Logic.Release();
}


void Game::DoTick( float timeDelta )
{
	m_Gui.DoTick( timeDelta );
	m_Logic.DoTick( timeDelta );
}






////////////////////////////////////////////////////////////////////////////////
//								main function								  //
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
	if( !theGame.Init() )
		return INVALID_VALUE;

	theGame.Run();

	theGame.Release();

	return VALID_VALUE;
}
