#include "Game.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

Game theGame;


int main( int argc, char** argv )
{
	if( !theGame.Init() )
		return -1;

	theGame.Run();

	theGame.Release();

	return 0;
}