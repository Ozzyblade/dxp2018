#ifndef StartModeH
#define StartModeH


#include "gameMode.h"

class StartMode : public gameMode
{
public:
	StartMode();
	virtual ~StartMode();

	void update(float dtime);
	void render(float dtime);

	virtual void enterState();
	virtual void exitState();
	virtual int nextMode() { return 1; }
	
	float changingNumber = 0;

	PlayerController mPlayer;

	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;

private:
	int menuChoice = 1;			// Start at the top of the menu - Start Game.
	bool menuBounce = false;	// Used for only acting once when a key is pressed, forcing a release before acting again.
};

#endif // !StartModeH

