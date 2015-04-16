
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