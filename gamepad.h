typedef enum
{
	GamePad_Button_DPAD_UP,
	GamePad_Button_DPAD_DOWN,
	GamePad_Button_DPAD_LEFT,
	GamePad_Button_DPAD_RIGHT,
	GamePad_Button_START,
	GamePad_Button_BACK,
	GamePad_Button_LEFT_THUMB,
	GamePad_Button_RIGHT_THUMB,
	GamePad_Button_LEFT_SHOULDER,
	GamePad_Button_RIGHT_SHOULDER,
	GamePad_Button_A,
	GamePad_Button_B,
	GamePad_Button_X,
	GamePad_Button_Y,
	GamePadButton_Max
}GamePadButton;
typedef enum
{
	GamePadIndex_One,
	GamePadIndex_Two,
	GamePadIndex_Three,
	GamePadIndex_Four,
}GamePadIndex;
struct GamePadState 
{
	bool		_buttons[GamePadButton_Max];
	float 	_left_thumbstick_x,_left_thumbstick_y;
	float 	_right_thumbstick_x,_right_thumbstick_y;
	float		_left_trigger;
	float		_right_trigger;	
        // Just to clear all values to default
	void reset()
	{
		for (int i=0;i<(int)GamePadButton_Max;++i) _buttons[i] = false;
		_left_thumbstick_x = 0.0f;
		_left_thumbstick_y = 0.0f;
		_right_thumbstick_x = 0.0f;
		_right_thumbstick_y = 0.0f;
		_left_trigger = _right_trigger = 0.0f;
	}
};
class GamePadXbox 
{
public: 
	GamePadXbox(GamePadIndex player)			
	{
                  _playerIndex = player;
                  State.reset();
	}
 
	virtual ~GamePadXbox(void)
	{
		// We don't want the controller to be vibrating accidentally when we exit the app
		if(checkConnected()) vibrate(0.0f,0.0f);
	}
 
	bool get_input();
	bool checkConnected();
    void vibrate(float leftmotor = 0.0f, float rightmotor = 0.0f);
	void update();
 
 public:
   GamePadState	State;
 private:
   XINPUT_STATE _controllerState;
   GamePadIndex _playerIndex;
 
};
extern bool previousPresses[GamePadButton_Max];
extern void getPreviousPress();
extern bool pad_button_held(GamePadButton btn);
extern bool pad_button_pressed(GamePadButton btn);