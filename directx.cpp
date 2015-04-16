#include "header.h"

// ## DIRECT X ## //
LPDIRECT3DDEVICE9 gD3dDevice = NULL;
D3DPRESENT_PARAMETERS presParams; 
WNDCLASSEX wcex;
HWND hWnd;
LPDIRECT3D9 gD3dObject = NULL;
D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_2_SAMPLES;

LPD3DXSPRITE gSprite[(int)MAX_LAYERS];
LPD3DXFONT font;
LPD3DXFONT font_large;

void clearScreen(float r, float g, float b)
{
	gD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB((int)r, (int)g, (int)b),NULL,NULL);
}
LRESULT CALLBACK WindowProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
		case WM_SETCURSOR:
			// Turn off window cursor.
			SetCursor( NULL );
			gD3dDevice->ShowCursor( TRUE );
			return TRUE; // Prevent Windows from setting cursor to window class cursor.
		break;
		case WM_COMMAND:
		{
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam); 

			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_EXIT:
				   DestroyWindow(wnd);
				break;
			}
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
   }
   return DefWindowProc(wnd, uMsg, wParam, lParam);

}
bool initDirectX(HINSTANCE hInstance, int nCmdShow)
{
	int i;
    bool windowed = WINDOW_MODE;
	if( NULL == (gD3dObject = Direct3DCreate9(D3D_SDK_VERSION)))
		return false;
	
	// CREATE A WINDOW
	ZeroMemory(&wcex,sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)WindowProc;												
	wcex.cbClsExtra = 0;												
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "MainWindow";
	if(windowed)
	{	
		// window mode
		wcex.hbrBackground = WINDOW_COLOR;

		if (RegisterClassEx(&wcex)==0)
			return false;
		//set the viewport size to WIDTH AND HEIGHT
		hWnd=CreateWindow(
				wcex.lpszClassName,
				WINDOW_NAME, 
				WS_OVERLAPPEDWINDOW  | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
				NULL,
				NULL,
				hInstance,
				NULL);
	} else
	{
		// full screen
		if (RegisterClassEx(&wcex)==0)
			return false;
		hWnd=CreateWindow(
			wcex.lpszClassName,
			WINDOW_NAME, 
			WS_EX_TOPMOST  | WS_POPUP,
			0, 0,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			NULL,
			NULL,
			hInstance,
			NULL);
	}
	if(!hWnd) return false;	
	
	// CREATE DIRECT X DEVICE
	ZeroMemory(&presParams,sizeof(D3DPRESENT_PARAMETERS));
	
	presParams.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	presParams.EnableAutoDepthStencil=1;
	presParams.AutoDepthStencilFormat=D3DFMT_D24S8;
	presParams.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
	presParams.PresentationInterval= D3DPRESENT_INTERVAL_IMMEDIATE;
	presParams.hDeviceWindow=hWnd;
	presParams.MultiSampleType=D3DMULTISAMPLE_2_SAMPLES;
	presParams.SwapEffect=D3DSWAPEFFECT_COPY;
	presParams.BackBufferFormat=D3DFMT_X8R8G8B8;
	presParams.BackBufferCount=1;
	
	presParams.BackBufferWidth=WINDOW_WIDTH;
	presParams.BackBufferHeight=WINDOW_HEIGHT;
	presParams.Windowed=windowed;


	// Create the D3D device
	HRESULT hr=gD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &presParams, &gD3dDevice);
	if (FAILED(hr))
	{
		// Some cards may not do hardware vertex processing so fall back to software:
		hr=gD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presParams, &gD3dDevice);
		bool goodMultiSample = true;
		if(FAILED(hr))
			goodMultiSample = false;
		while (!goodMultiSample)
		{
			// some devices do not support multi sampling, downgrade until supported
			multiSampleType = (D3DMULTISAMPLE_TYPE)((int)multiSampleType - 1);
			presParams.MultiSampleType = multiSampleType;
			hr=gD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presParams, &gD3dDevice);
			if (FAILED(hr) && (presParams.MultiSampleType == 0))
			{
				MessageBox("Failed to create 3D device","",MB_OK);
				gameState = GAME_EXIT;
				return false;
			} else if(!FAILED(hr))
			{
				goodMultiSample = true;
			}
		}
	}
	
	// DISPLAY THE WINDOW
	ShowWindow(hWnd, nCmdShow);

	// CREATE THE LAYER SPRITES
	for(i=0;i<MAX_LAYERS;i++)
	{
		HRESULT hr=D3DXCreateSprite(gD3dDevice,&gSprite[i]);
		if (FAILED(hr))
		{
			MessageBox("Failed to D3D sprite","",MB_OK);
			return false;
		}
	}

	// INIT THE DEFAULT FONT
	D3DXCreateFont(gD3dDevice, FONT_SIZE, 0, 1, NULL, false, DEFAULT_CHARSET,  OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "SSF4 ABUKET", &font);
	D3DXCreateFont(gD3dDevice, FONT_SIZE_LARGE, 0, 1, NULL, false, DEFAULT_CHARSET,  OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "SSF4 ABUKET", &font_large);
	return true;
}
void cleanupDirectX()
{
	for(int i=0; i < MAX_TEXTURES;i++)
	{
		if(textures[i].tex)
			textures[i].tex->Release();
	}
	for(int i=0;i<MAX_LAYERS;i++)
	{
		if(gSprite[i])
			gSprite[i]->Release();
	}
	if (gD3dObject)
		gD3dObject->Release();

	if (gD3dDevice)
		gD3dDevice->Release();

}
void renderSprite(Sprites * spr)
{
	float rotz = 0.0f;

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXVECTOR2 scaling(spr->scaleX, spr->scaleY);
	D3DXVECTOR2 pos(spr->x+BORDER_X,spr->y + BORDER_Y);
	D3DXVECTOR2 center(spr->x+((spr->w)/2),spr->y+((spr->h)/2));
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&center,rotz,&pos);
	gSprite[spr->layer]->SetTransform(&mat);
	RECT drawRect;
	int x=0;
	int y=0;
	float offset = floor(spr->frame) * spr->w;
	for(float j = 0; j < offset; j += spr->w)
	{
		x += spr->w;
		if(x >= textures[spr->texId].w)
		{
			x = 0;
			y += spr->h;
		}
	}
	
	drawRect.left=x;
	drawRect.right=drawRect.left+spr->w;
	drawRect.top=y;
	drawRect.bottom=drawRect.top+spr->h;
	gSprite[spr->layer]->Draw(textures[spr->texId].tex,&drawRect,NULL,NULL,spr->renderColor);
}
void renderSprites()
{
	for(int i = 0;i<MAX_SPRITES;i++)//loop through sprites
	{		
		if(!sprites[i].used)
			continue;
		renderSprite(&sprites[i]);
	}
}
void renderText()
{	
	int i;
	for(i=0;i< MAX_TEXT_SPRITES;i++)
	{
		if(!text_sprites[i].used)
			continue;
		RECT rect = RECT();
		rect.left = (int)text_sprites[i].x + BORDER_X;
		rect.right = (int)(text_sprites[i].x + WINDOW_WIDTH) + BORDER_X;
		rect.top = (int)text_sprites[i].y + BORDER_Y;
		rect.bottom = (int)(text_sprites[i].y + WINDOW_HEIGHT) + BORDER_Y;
		text_sprites[i].font->DrawTextA(gSprite[LAYER_TEXT], text_sprites[i].str, strlen(text_sprites[i].str), &rect, DT_LEFT, D3DCOLOR_XRGB((int)text_sprites[i].r,(int) text_sprites[i].g, (int)text_sprites[i].b));
	}	
}
void render()
{
	gD3dDevice->BeginScene();
	clearScreen(0,0,0);

	int i;
	for(i = 0; i < MAX_LAYERS;i++)
	{
		gSprite[i]->Begin(D3DXSPRITE_ALPHABLEND);
	}
	renderText();
	renderSprites();
	for(i = 0; i < MAX_LAYERS;i++)
	{
		gSprite[i]->End();
	}
	gD3dDevice->EndScene();
	gD3dDevice->Present( NULL, NULL, NULL, NULL );
}

