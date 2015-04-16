#include "header.h"

// ## GENERAL ## //
Textures textures[MAX_TEXTURES];
Sprites sprites[MAX_SPRITES];
TextSprites text_sprites[MAX_TEXT_SPRITES];
Background bgs[LAYER_MAX_BGS];

int LAST_FREE_TEXTURE = 0;
int LAST_FREE_SPRITE = 0;

void initTexture(int i)
{
		textures[i].used = false;
		strcpy_s(textures[i].fn,"");
		textures[i].h = 0;
		textures[i].w = 0;
		textures[i].id = 0;
		textures[i].tex = 0x00000000;
}
void initTextures()
{
	int i;
	for(i = 0;i<MAX_TEXTURES;i++)
	{
		if(!textures[i].used)
			continue;
		initTexture(i);
	}
}
void initSprite(Sprites *spr)
{
		spr->used = false;
		spr->texId = -1;
		spr->w = 0;
		spr->h = 0;
		spr->x = 0;
		spr->y = 0;
		spr->scaleX = 1.00f;
		spr->scaleY = 1.00f;
		spr->id = 0;
		spr->alive = false;
		spr->frame = 0;	
}

void initSprites()
{
	int i;
	for(i = 0;i<MAX_SPRITES;i++)
	{
		initSprite(&sprites[i]);
	}
}
void initGraphics()
{
	initTextures();
	initSprites();
}
int getFreeTexture(char * fn)
{
	int i;
	for(i = 0;i<MAX_TEXTURES;i++)
	{
		if(strcmp(fn,textures[i].fn) == 0)
				return i;
	}
	for(i = 0;i<MAX_TEXTURES;i++)
	{
		if(!textures[i].used)
				return i;
	}
	return -1;
}
int getFreeTextSprite(char * text)
{	
	for(int i = 0;i<MAX_TEXT_SPRITES;i++)
	{
		if(strcmp(text_sprites[i].str,text) == 0)
			return i;
	}
	for(int i = 0;i<MAX_TEXT_SPRITES;i++)
	{
		if(!text_sprites[i].used)
			return i;
	}
	return -1;
}
int getFreeSprite()
{
	for(int i = 0;i<MAX_SPRITES;i++)
	{
			if(!sprites[i].used)
				return i;
	}
	return -1;
}
int createTexture(char * fn, int w, int h)
{
	int tex = getFreeTexture(fn);
	if(tex < 0)
		return -1;
	if(!textures[tex].used)
	{
		textures[tex].used = true;
		textures[tex].w = w;
		textures[tex].h = h;
		textures[tex].id = tex;
		strcpy_s(textures[tex].fn, fn);
		HRESULT hr = D3DXCreateTextureFromFileEx(gD3dDevice, fn, D3DX_DEFAULT_NONPOW2, 
			D3DX_DEFAULT_NONPOW2, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
             0, NULL, NULL, &textures[tex].tex);
		if(FAILED(hr))
		{
			MessageBox("Failed to create texture", "Error",NULL);
			gameState = GAME_EXIT;
		}
	}
	return tex;
}
int createSprite(int texId, int layer, int w, int h, int imgW, int imgH, int x, int y, int renderColor)
{
	int spr = getFreeSprite();
	if(spr < 0)
	{
		MessageBox("No free sprites.", "Error",NULL);
		return -1;
	}	
		sprites[spr].used = true;
		sprites[spr].texId = texId;
		sprites[spr].w = w;
		sprites[spr].h = h;
		sprites[spr].imgW = imgW;
		sprites[spr].imgH = imgH;
		sprites[spr].layer = layer;
		sprites[spr].x = (float)x;
		sprites[spr].y = (float)y;
		sprites[spr].id = spr;
		sprites[spr].renderColor = renderColor;
		sprites[spr].frame = 0;
		sprites[spr].alive = true;
		return spr;

}
void disposeSprite(int id)
{
	initSprite(&sprites[id]);
}
void disposeSprites()
{
	for(int i = 0;i<MAX_SPRITES;i++)
	{
		if(!sprites[i].used)
			continue;
		disposeSprite(i);
	}
}
void disposeTexture(int id)
{
	textures[id].tex->Release();
	initTexture(id);
}
void disposeTextures()
{
	for(int i = 0;i<MAX_TEXTURES;i++)
	{
		if(!textures[i].used)
			continue;
		disposeTexture(i);
	}
}
void disposeBackground()
{
	for(int i = 0;i<LAYER_MAX_BGS;i++)
	{
		if(!bgs[i].used)
			continue;
		bgs[i].used = false;

	}
}
int getFreeBackground()
{
	for(int i = 0;i<LAYER_MAX_BGS;i++)
	{
		if(!bgs[i].used)
			return i;
	}
	return -1;
}
int createBackground(int texId, float scrollX, float scrollY, layersType layer)
{
	int bg = getFreeBackground();
	bgs[bg].used = true;
	bgs[bg].scrollX = scrollX;
	bgs[bg].scrollY = scrollY;
	bgs[bg].texId = texId;
	bgs[bg].x = -WINDOW_WIDTH-BORDER_X;
	bgs[bg].y = -WINDOW_HEIGHT-BORDER_Y;
	int k =0;
	for(int y=0;y<3;y++)
	{
		for(int x=0;x<3;x++)
		{
			int i = createSprite(texId, layer, WINDOW_WIDTH, WINDOW_HEIGHT,800, 600, (x*WINDOW_WIDTH)-BORDER_X, (y*WINDOW_HEIGHT)-BORDER_Y, 0xffffffff);
			bgs[bg].spr[k] = &sprites[i];
			k++;
		}
	}
	return bg;
}
void doScrollingBackgroundLogic()
{
	for(int bg = 0; bg<LAYER_MAX_BGS;bg++)
	{
	int x = 0;
	int y = 0;
		for(int k = 0; k<BG_MAX_BGLOC;k++)
		{
			bgs[bg].x += bgs[bg].scrollX;
			bgs[bg].y += bgs[bg].scrollY;
			if(bgs[bg].scrollX == 0 && bgs[bg].scrollY == 0 || !bgs[bg].used)
				continue;
			if(bgs[bg].x <= -(WINDOW_WIDTH*2) - BORDER_X || bgs[bg].x >= BORDER_X)
				bgs[bg].x = -WINDOW_WIDTH - BORDER_X;
			if(bgs[bg].y <= -(WINDOW_HEIGHT*2) - BORDER_Y || bgs[bg].y >= BORDER_Y)
				bgs[bg].y = -WINDOW_HEIGHT - BORDER_Y;
			bgs[bg].spr[k]->x=bgs[bg].x+(x*WINDOW_WIDTH); 
			bgs[bg].spr[k]->y=bgs[bg].y+(y*WINDOW_HEIGHT);
			x++;
			if(x >= 3)
			{
				x-=3;
				y++;
			}
		}

	}
}
int printString(char * str, float x, float y, float r, float g, float b, LPD3DXFONT textfont)
{
	int sprite = getFreeTextSprite(str);
	if(sprite < 0)
	{
		MessageBox("No free text sprites.", "Error", NULL);
		return -1;
	}
	strcpy_s(text_sprites[sprite].str, str);
	text_sprites[sprite].x = x;
	text_sprites[sprite].y = y;
	text_sprites[sprite].r = r;
	text_sprites[sprite].g = g;
	text_sprites[sprite].b = b;
	text_sprites[sprite].font = textfont;
	text_sprites[sprite].used = true;
	return sprite;
}

