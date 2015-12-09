#ifndef __H__GAME_H__
#define __H__GAME_H__
#include "GameGui.h"
#include "GameLogic.h"

class Game
{
public:
	bool			Init();

	void			Run();

	void			Release();

private:
	void			DoTick( float timeDelta );

private:
	GameGui			m_Gui;
	GameLogic		m_Logic;
};


extern Game theGame;



#endif // __H__GAME_H__