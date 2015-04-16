#include "header.h"
FMOD_SYSTEM      *fmod_system;
FMOD_CHANNEL     *fmod_channel = 0;
FMOD_RESULT       fmod_result;
FModSound fmod_snd[MAX_SOUNDS];

void initFmodSnd()
{
	for(int i = 0;i<MAX_SOUNDS;i++)
	{
		fmod_snd[i].used = false;
	}
}
bool disposeFmodSnd(int sound)
{
	fmod_snd[sound].used = false;
	fmod_result = FMOD_Sound_Release(fmod_snd[sound].fmod_sound);
	if(!checkFmodError("fmod_ReleaseSounds 001"))
		return false;
	return true;
}
bool checkFmodError(char * funcName)
{
	if(fmod_result != FMOD_OK)
	{
		char str[256] = "An error has occured. Technical reference: ";
		strcat_s(str, funcName); 
		MessageBox(str, "FMOD Error", MB_ICONERROR);
		return false;
	}
	return true;
}
int getFreeSound()
{
	for(int i = 0; i<MAX_SOUNDS;i++)
	{
		if(fmod_snd[i].used)
			continue;
		return i;
	}
	return -1;
}
bool initFmod()
{
	initFmodSnd();
	if(!fmod_createSystem())
		return false;
	return true;
}

bool fmod_createSystem()
{
	fmod_result = FMOD_System_Create(&fmod_system);
	if(!checkFmodError("fmod_createSystem 001"))
		return false;	
    fmod_result = FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, NULL);
    if(!checkFmodError("fmod_createSystem 002"))
		return false;
	return true;
}
int fmod_initSound(char * fn, int sfx)
{
	int i = sfx;
	if(sfx < 0)
		i = getFreeSound();
	else
	{
		if(fmod_snd[i].used)
			disposeFmodSnd(i);
	}
	if(i < 0)
	{
		MessageBox("No free sounds.", "Error", MB_ICONERROR);
		return -1;
	}
	fmod_result = FMOD_System_CreateSound(fmod_system, fn, FMOD_HARDWARE, 0, &fmod_snd[i].fmod_sound);
	if(!checkFmodError("fmod_initSound 001"))
		return -1;
	fmod_snd[i].used = true;
	return i;
}
bool fmod_playSound(FMOD_MODE mode, int sound, bool pause)
{
	fmod_result = FMOD_Sound_SetMode(fmod_snd[sound].fmod_sound, mode);
	if(!checkFmodError("fmod_playSound 001"))
		return false;	
	fmod_result = FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmod_snd[sound].fmod_sound, pause, &fmod_channel);
	if(!checkFmodError("fmod_playSound 002"))
		return false;
	return true;
}
bool fmod_Update()
{
	fmod_result = FMOD_System_Update(fmod_system);
	if(!checkFmodError("fmod_Update 001"))
		return false;
	return true;
}
bool fmod_ReleaseSounds()
{
	for(int i= 0;i < MAX_SOUNDS;i++)
	{
		if(!fmod_snd[i].used)
			continue;
		fmod_result = FMOD_Sound_Release(fmod_snd[i].fmod_sound);
		if(!checkFmodError("fmod_ReleaseSounds 001"))
			return false;
	}
	return true;
}
bool fmod_Shutdown()
{
	fmod_ReleaseSounds();
	fmod_result = FMOD_System_Close(fmod_system);
	if(!checkFmodError("fmod_Shutdown 001"))
		return false;
	fmod_result = FMOD_System_Release(fmod_system);
	if(!checkFmodError("fmod_Shutdown 002"))
		return false;
	return true;
}