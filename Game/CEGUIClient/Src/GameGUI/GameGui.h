#ifndef __H_CEGUI_APP_H__
#define __H_CEGUI_APP_H__
#include "Common.h"
#include "ConstDef.h"
#include "CEGUI.h"
#include "CEGUIDirect3D9Renderer.h"
#include "Direct3D9Init.h"
#include "GameWnd.h"
using namespace CEGUI;




class GameGui : public Input
{
public:
	GameGui();

public:
	bool				Init();

	void				Release();

	void				DoTick( float timeDelta );

protected:
	virtual void		HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void				Render( float timeDelta );

	void				InitialiseResourceGroupDirectories();

	void				InitialiseDefaultResourceGroups();

	bool				CheckDeviceLost();

private:
	PDirect3DDevice		m_Device;
	Direct3D9Renderer*	m_Renderer;
	CEGUI::System*		m_GUISystem;
};



#endif // __H_CEGUI_APP_H__