#ifndef __H_CEGUI_APP_H__
#define __H_CEGUI_APP_H__
#include "Common.h"
#include "CEGUI.h"
#include "CEGUIDirect3D9Renderer.h"
#include "Direct3D9Init.h"
#include "GameWnd.h"
using namespace CEGUI;




class GameGui : public EventSet, public Input
{
public:
	bool				Init();

	virtual void		HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void				Render( float timeDelta );

	void				DoTick( float timeDelta );

protected:
	void				InitialiseResourceGroupDirectories();

	void				InitialiseDefaultResourceGroups();

	const char*			GetDataPathPrefix() const { return m_DataPathPrefix; }

	bool				checkDeviceLost();

private:
	PDirect3DDevice		m_Device;
	Direct3D9Renderer*	m_Renderer;
	CEGUI::System*		m_GUISystem;
	char				m_DataPathPrefix[COMMON_CHAR_BUF_128];
};



#endif // __H_CEGUI_APP_H__