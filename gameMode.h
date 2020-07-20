#ifndef gameModeH
#define gameModeH

#include <CommonStates.h>
//purely virtual to use with late binding for game modes
#include "PlayerController.h"
#include "HeavyDrone.h"
#include "LightDrone.h"
#include "bullet.h"
#include "Interactable.h"
#include "WindowUtils.h"
#include "Level.h"

class gameMode {
public:
	gameMode();
	virtual ~gameMode();

	virtual void update(float dtime) = 0;
	virtual void render(float dtime) = 0;

	virtual void enterState() = 0;
	virtual void exitState() = 0;
	virtual int nextMode() = 0;
	virtual void getStats(float&, float&, int&);
	virtual void handleKey(char);
	bool endMode() const { return modeOver; }

	bool modeOver;
};


#endif
