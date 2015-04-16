#include "header.h"

bool ExitGame = false;
bool handleKeyEvent(LPMSG lpMsg)
{
	if ((lpMsg->message == WM_KEYDOWN))
	{
		switch(lpMsg->wParam)
		{
		case VK_ESCAPE:
				if(gameState == GAME_PLAY)
				{
					gameState = INIT_GAME_PAUSE;
					return true;
				}
			break;
		case VK_F1:
			if(gameState == GAME_PLAY)
			{
				resizeBat();
				return true;
			}
			break;
			case VK_SPACE:
				if(gameState == GAME_PLAY)
				{
					for(int i = 0;i<MAX_BALLS;i++)
					{
						if(ball[i].velocity == 0)
						{
							ball[i].xAdjust = player.cur_speed;
							ball[i].velocity = powerUp[PWRUP_PADDLE_STICKY].param[1];
						}
					}
				}
				if(gameState == GAME_PAUSE)
				{
					gameState = GAME_PLAY;
					return true;
				}
				if(gameState ==  GAME_NXT_LVL)
				{
					curStage++;
					beginScreenFader(200, false);					
					if(curStage > MAX_STAGES)
					{
						gameState = INIT_GAME_OVER;
						return true;
					}
					gameState = INIT_GAME_PLAY;
					return true;
				}
				break;
			case VK_UP:
				if(gameState == MAIN_MENU)
				{
					mainMenuSelected--;
					if(mainMenuSelected < 0)
						mainMenuSelected = 2;
					return true;
				}
				break;
			case VK_DOWN:
				if(gameState == MAIN_MENU)
				{
					mainMenuSelected++;
					if(mainMenuSelected > 2)
						mainMenuSelected = 0;
					return true;
				}
				break;
			case VK_RETURN:
				if(gameState == MAIN_MENU)
				{
					MainMenuSelectOption();
					return true;
				}
				if(gameState == GAME_OVER)
				{
					beginScreenFader(200, false);
					gameState = INIT_MAIN_MENU;
					return true;
				}
				break;
		}
	}
	return false;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	srand ((int)time(NULL));
	if(!initDirectX(hInstance, nCmdShow))
		return -1;
	
	initGraphics();

	MSG message;
    ZeroMemory( &message, sizeof(message) );
	unsigned __int64 CurTicks = GetTickCount64();
	unsigned __int64 nextTick = CurTicks + (1000 / FPS);
	while(( message.message!=WM_QUIT )&&(!ExitGame))
	{
		if( PeekMessage( &message, NULL, 0U, 0U, PM_REMOVE ) )
        {
			bool fHandled = false;
			if (message.message >= WM_KEYFIRST && message.message <= WM_KEYLAST)
				fHandled = handleKeyEvent(&message);

			if (!fHandled)
			{
				TranslateMessage( &message );
				DispatchMessage( &message );
			}
        }
        else 
		{
			if(!ScreenFadeLogic())
				updateLogic((message.message == WM_KEYDOWN), &message);
			while(CurTicks < nextTick)
			{
				doScrollingBackgroundLogic();
				drawLogic();
				CurTicks = GetTickCount64();
			}
			nextTick = CurTicks + (1000 / FPS);
		}
	}	
	cleanupDirectX();
	return message.wParam;
}