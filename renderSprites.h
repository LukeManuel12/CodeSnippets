#define MAX_TEXTURES 12
#define MAX_SPRITES 2000
#define MAX_TEXT_SPRITES 100

enum layersType
{
	LAYER_BG0, // change these relative to MAX_BGS
	LAYER_BG1,
	LAYER_BG2,
	LAYER_MAX_BGS,
	LAYER_BT_SPRITES,
	LAYER_TEXT,
	LAYER_IT_SPRITES,
	LAYER_BORDER_OBJ,
	LAYER_BORDER,
	LAYER_TOP,
	MAX_LAYERS, // always at the end
};

typedef struct
{
	int w,h,id;
	char fn[1024];
	bool used;
	
	IDirect3DTexture9* tex;
} Textures;
typedef struct 
{
	int id, texId; 
	float x, y; 
	int w, h;
	int imgW, imgH;
	bool used;
	bool alive;
	int layer;
	float frame;
	int renderColor;
	float scaleX;
	float scaleY;
} Sprites;
typedef struct
{
	char str[256]; 
	float x; 
	float y; 
	float r; 
	float g; 
	float b; 
	bool used;
	LPD3DXFONT font;
} TextSprites;
enum Backgrounds
{
	BG_TOP_LEFT,
	BG_TOP_CENTER,
	BG_TOP_RIGHT,
	BG_MIDDLE_LEFT,
	BG_MIDDLE_CENTER,
	BG_MIDDLE_RIGHT,
	BG_BOTTOM_LEFT,
	BG_BOTTOM_CENTER,
	BG_BOTTOM_RIGHT,
	BG_MAX_BGLOC
};
typedef struct
{
	float scrollX;
	float scrollY;
	int texId;
	bool used;
	float x, y;
	Sprites * spr[9];
	layersType layer;
} Background;

extern Textures textures[MAX_TEXTURES];
extern Sprites sprites[MAX_SPRITES];
extern TextSprites text_sprites[MAX_TEXT_SPRITES];
extern Background bgs[LAYER_MAX_BGS];
