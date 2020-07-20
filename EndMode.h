#ifndef EndModeH
#define EndModeH

#include "gameMode.h"
#include "scoreUsername.h"
#include <vector>

class EndMode : public gameMode
{
public:
	EndMode();
	virtual ~EndMode();

	void update(float dTime);
	void render(float dTime);

	void enterState();
	void exitState();
	void getStats(float&, float&, int&);
	int nextMode() { return 0; }
	void handleKey(char);

	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;
private:
	void loadScores();
	wstring name;
	wstring winLoss;
	scoreUsername highScr;
	int oX, oY;
	float playerScore;
	bool nameEntered = false;
};


#endif

