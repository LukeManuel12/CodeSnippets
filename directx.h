// ## DIRECT X ## //
#define FONT_SIZE 40
#define FONT_SIZE_LARGE 50

extern LPDIRECT3DDEVICE9 gD3dDevice;
extern D3DPRESENT_PARAMETERS presParams; 
extern WNDCLASSEX wcex;
extern HWND hWnd;
extern LPDIRECT3D9 gD3dObject;
extern D3DMULTISAMPLE_TYPE multiSampleType;
extern LPD3DXSPRITE gSprite[(int)MAX_LAYERS];
extern LPD3DXFONT font;
extern LPD3DXFONT font_large;

extern void render();
extern bool initDirectX(HINSTANCE hInstance, int nCmdShow);
extern void cleanupDirectX();