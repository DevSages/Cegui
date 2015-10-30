#include "Game.h"


Game theGame;


int main( int argc, char** argv )
{
	if( !theGame.Init() )
		return INVALID_VALUE;

	theGame.Run();

	theGame.Release();

	return VALID_VALUE;
}
