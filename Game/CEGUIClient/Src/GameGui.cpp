#include "GameGui.h"


bool GameGui::Init()
{
	GameWnd& wnd = GameWnd::GetWnd();
	if( !wnd.InitWnd( 800, 600, false ) )
		return false;

	DeviceWnd d_wnd;
	ZeroMemory( &d_wnd, sizeof( DeviceWnd ) );
	d_wnd.d_bFullScreen = wnd.m_bFullScreen;
	d_wnd.d_width = wnd.m_Width;
	d_wnd.d_height = wnd.m_Height;
	d_wnd.d_hwnd = wnd.m_Hwnd;
	d_wnd.d_hinst = wnd.m_Hinst;

	m_Device = NULL;
	if( !InitDevice( d_wnd, &m_Device ) || m_Device == NULL )
		return false;

	wnd.ShowWindow();
	wnd.AddInputHander( this );

	m_Renderer = &Direct3D9Renderer::bootstrapSystem( m_Device->d_device );
	if( !m_Renderer )
		return false;

	if( ( m_GUISystem = CEGUI::System::getSingletonPtr() ) == NULL )
		return false;

	InitialiseResourceGroupDirectories();
	InitialiseDefaultResourceGroups();

	return true;
}


void GameGui::HandleInput( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return;

	switch( uMsg )
	{
	case WM_CHAR:
		{
			System::getSingleton().injectChar((CEGUI::utf32)wParam);
			DestroyWindow( GameWnd::GetWnd().m_Hwnd );
		}
		break;

	case WM_MOUSEMOVE:
		{
			System::getSingleton().injectMousePosition((float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
		}
		break;

	case WM_LBUTTONDOWN:
		{
			System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
		}
		break;

	case WM_LBUTTONUP:
		{
			System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
		}
		break;

	case WM_RBUTTONUP:
		{
			System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
		}
		break;

	case WM_MBUTTONDOWN:
		{
			System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
		}
		break;

	case WM_MBUTTONUP:
		{
			System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			System::getSingleton().injectMouseWheelChange(static_cast<float>((short)HIWORD(wParam)) / static_cast<float>(120));
		}
		break;

	case WM_SIZE:
		{
			CEGUI::System* cegui_system = CEGUI::System::getSingletonPtr();
			if ((cegui_system != 0) && (wParam != SIZE_MINIMIZED))
			{
				CEGUI::Renderer* renderer = cegui_system->getRenderer();
				CEGUI::String id(renderer->getIdentifierString());
			}
		}
		break;
	}
}

void GameGui::Render( float timeDelta )
{
	if( m_GUISystem != NULL )
	{
		m_GUISystem->renderGUI();

		m_GUISystem->injectTimePulse( timeDelta );
		if( !checkDeviceLost() )
		{
			
		}
	}
}

void GameGui::DoTick( float timeDelta )
{
	if( m_GUISystem == NULL )
		return;
	Render( timeDelta );
}

void GameGui::InitialiseResourceGroupDirectories()
{
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

	const char* dataPathPrefix = GetDataPathPrefix();
	char resourcePath[PATH_MAX];

	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "schemes/" );
	rp->setResourceGroupDirectory("schemes", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
	rp->setResourceGroupDirectory("imagesets", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
	rp->setResourceGroupDirectory("fonts", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
	rp->setResourceGroupDirectory("layouts", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
	rp->setResourceGroupDirectory("looknfeels", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
	rp->setResourceGroupDirectory("lua_scripts", resourcePath);
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
	rp->setResourceGroupDirectory("schemas", resourcePath);   
	sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "animations/");
	rp->setResourceGroupDirectory("animations", resourcePath); 
}

void GameGui::InitialiseDefaultResourceGroups()
{
	// set the default resource groups to be used
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	CEGUI::AnimationManager::setDefaultResourceGroup("animations");

	// setup default group for validation schemas
	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");
}


bool GameGui::checkDeviceLost()
{
	HRESULT coop = m_Device->d_device->TestCooperativeLevel();

	if( coop == D3DERR_DEVICELOST )
	{
		return true;
	}
	else if( coop == D3DERR_DEVICENOTRESET )
	{
		m_Renderer->preD3DReset();
		if( SUCCEEDED( m_Device->d_device->Reset(&m_Device->d_d3dPresentParam) ) )
		{
			m_Renderer->postD3DReset();
			return false;
		}
	}
	return false;
}