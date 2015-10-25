#define NOMINMAX
#include "Direct3D9Init.h"
#include "CEGUIExceptions.h"

namespace CEGUI
{
	DIRECT3D9_GUIRENDERER_API bool	InitDevice( const DeviceWnd& wnd, PDirect3DDevice* pOutDev )
	{
		HRESULT hr  = 0;

		if( (*pOutDev = new Direct3DDevice) == NULL )
			return false;
		PDirect3DDevice device = *pOutDev;


		device->d_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
		if( device->d_d3d9 == NULL )
		{
			return false;
		}

		D3DCAPS9 caps;
		device->d_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

		int vp = 0;
		if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


		device->d_d3dPresentParam.BackBufferWidth            = wnd.d_width;
		device->d_d3dPresentParam.BackBufferHeight           = wnd.d_height;
		device->d_d3dPresentParam.BackBufferFormat           = D3DFMT_A8R8G8B8;
		device->d_d3dPresentParam.BackBufferCount            = 1;
		device->d_d3dPresentParam.MultiSampleType            = D3DMULTISAMPLE_NONE;
		device->d_d3dPresentParam.MultiSampleQuality         = 0;
		device->d_d3dPresentParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
		device->d_d3dPresentParam.hDeviceWindow              = wnd.d_hwnd;
		device->d_d3dPresentParam.Windowed                   = !wnd.d_bFullScreen;
		device->d_d3dPresentParam.EnableAutoDepthStencil     = true; 
		device->d_d3dPresentParam.AutoDepthStencilFormat     = D3DFMT_D24S8;
		device->d_d3dPresentParam.Flags                      = 0;
		device->d_d3dPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		device->d_d3dPresentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


		hr = device->d_d3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd.d_hwnd, vp, &device->d_d3dPresentParam, &device->d_device );   

		if( FAILED(hr) )
		{
			device->d_d3dPresentParam.AutoDepthStencilFormat = D3DFMT_D16;

			hr = device->d_d3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd.d_hwnd, vp, &device->d_d3dPresentParam, &device->d_device );

			if( FAILED(hr) )
			{
				device->d_d3d9->Release();
				return false;
			}
		}
		return true;
	}
}