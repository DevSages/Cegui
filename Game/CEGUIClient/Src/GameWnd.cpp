#include "GameWnd.h"
#include "IniFile.h"
#include <tchar.h>

LPCTSTR szClassName = _TEXT( "GAME" );

GameWnd::Input_Handelers GameWnd::s_InputHandlers;


GameWnd& GameWnd::GetWnd()
{
	static GameWnd _singleton;
	return _singleton;
}


void GameWnd::AddInputHander( Input* pInputHandler )
{
	assert( pInputHandler );
	s_InputHandlers.insert( pInputHandler );
}

bool GameWnd::Init()
{
	IniFile config( DEF_CONFIG );
	int width = config.GetInteger( "window", "width" );
	int height = config.GetInteger( "window", "height" );
	bool bFullScreen = config.GetBool( "window", "bFullScreen" );

	WNDCLASS wc;
	ZeroMemory( &wc, sizeof( wc ) );

	wc.hbrBackground	=	(HBRUSH)GetStockObject( WHITE_BRUSH );
	wc.hCursor			=	LoadCursor( NULL, IDC_ARROW );
	wc.hIcon			=	LoadIcon( NULL, IDI_APPLICATION );
	wc.hInstance		=	GetModuleHandle( NULL );
	wc.lpfnWndProc		=	WndProc;
	wc.lpszClassName	=	szClassName;
	wc.lpszMenuName		=	NULL;
	wc.style			=	CS_HREDRAW | CS_VREDRAW;

	if( !RegisterClass( &wc ) )
		return false;

	HWND hwnd = NULL;
	if( bFullScreen )
	{
		hwnd = CreateWindow( szClassName, _TEXT(""), WS_EX_TOPMOST|WS_VISIBLE|WS_POPUP, 0, 0, width, height, NULL, NULL, wc.hInstance, NULL );
	}
	else
	{
		hwnd = CreateWindow( szClassName, _TEXT(""), WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, width, height, NULL, NULL, wc.hInstance, NULL );
	}
	
	if( hwnd == NULL )
		return false;

	m_Hwnd	= hwnd;
	m_Hinst = wc.hInstance;
	m_Width = width;
	m_Height = height;
	m_bFullScreen = bFullScreen;

	return true;
}

void GameWnd::ShowWindow()
{
	::ShowWindow( m_Hwnd, SW_SHOW );
	::UpdateWindow( m_Hwnd );
}

void GameWnd::HideWindow()
{
	::ShowWindow( m_Hwnd, SW_HIDE );
}

void GameWnd::DestroyWnd()
{
	UnregisterClass( szClassName, m_Hinst );
}

LRESULT CALLBACK GameWnd::WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CLOSE:
		{
			DestroyWindow( GetWnd().m_Hwnd );
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );
		}
		break;
	default:
		{
			for( Input_HandlersIter it = s_InputHandlers.begin(); it != s_InputHandlers.end(); it++ )
			{
				Input* pInput = *it;
				if( pInput != NULL )
				{
					pInput->HandleInput( uMsg, wParam, lParam );
				}
			}
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
		break;
	}
	return 0;
}