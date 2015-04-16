
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