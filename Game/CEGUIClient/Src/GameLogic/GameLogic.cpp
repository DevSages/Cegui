#include "GameLogic.h"
#include "ResourceMngr.h"




bool GameLogic::Init()
{
	GenInstances();

	//bRtn = ResourceMngr::GetInstPtr()->Init();
	
	return true;
}

void GameLogic::Release()
{

}

void GameLogic::DoTick( float timeDelta )
{

}


void GameLogic::GenInstances()
{
	assert( new ResourceMngr() );
};