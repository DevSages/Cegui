#ifndef __H__GAME_H__
#define __H__GAME_H__
#include "GameGui.h"

class Game : public Input
{
public:
	bool			Init();

	void			Run();

	void			Release();

private:
	void			DoTick( float timeDelta );
	
	virtual void	HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	GameGui			m_Gui;
};


extern Game theGame;



#endif // __H__GAME_H__