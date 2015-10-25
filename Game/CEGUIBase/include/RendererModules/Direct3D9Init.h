#ifndef __H_DIRECT3D9INIT_H__
#define __H_DIRECT3D9INIT_H__

#include "CEGUIDirect3D9Renderer.h"



namespace CEGUI
{

	typedef struct tagDeviceWnd
	{
		HWND		d_hwnd;
		HINSTANCE	d_hinst;
		UINT		d_width;
		UINT		d_height;
		bool		d_bFullScreen;
	} DeviceWnd;

	typedef struct tagDevice
	{
		LPDIRECT3D9				d_d3d9;
		LPDIRECT3DDEVICE9		d_device;
		D3DPRESENT_PARAMETERS	d_d3dPresentParam;
	} Direct3DDevice, *PDirect3DDevice;


	extern DIRECT3D9_GUIRENDERER_API bool	InitDevice( const DeviceWnd& wnd, PDirect3DDevice* pOutDev );
}

#endif //__H_DIRECT3D9INIT_H__ 