#ifndef __H_GAME_LOGIC_H__
#define __H_GAME_LOGIC_H__
#include "Common.h"



class GameLogic
{
public:
	bool				Init();

	void				Release();

	void				DoTick( float timeDelta );

private:
	void				GenInstances();
};





#endif // __H_GAME_LOGIC_H__