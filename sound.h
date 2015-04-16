#define MAX_SOUNDS 10
enum SFX
{
	SFX_BGM,
	SFX_BLOCK_BREAK,
	SFX_BALL_BOUNCE
};
struct FModSound
{
	FMOD_SOUND       *fmod_sound;
	bool used;
};

extern FModSound fmod_snd[MAX_SOUNDS];
extern FMOD_SYSTEM      *fmod_system;
extern FMOD_CHANNEL     *fmod_channel;
extern FMOD_RESULT       fmod_result;

extern bool checkFmodError(char * funcName);
extern bool initFmod();

extern bool fmod_createSystem();
extern int fmod_initSound(char * fn, int sfx = -1);
extern bool fmod_playSound(FMOD_MODE mode, int sound, bool pause);
extern bool fmod_Update();
extern bool fmod_ReleaseSounds();
extern bool fmod_Shutdown();