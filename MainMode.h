#ifndef MainModeH
#define MainModeH

#include <vector>
#include<CommonStates.h>

#include "gameMode.h"
#include "PlayerController.h"
#include "HeavyDrone.h"
#include "LightDrone.h"
#include "bullet.h"
#include "Interactable.h"
#include "WindowUtils.h"
#include "Level.h"
#include "AmmoPickup.h"
#include "HealthPickup.h"
#include "Objective.h"
#include "EntityHandler.h"



class MainMode : public gameMode
{
public:
	MainMode();
	virtual ~MainMode();

	void update(float dtime) override;
	void render(float dtime) override;

	virtual void enterState();
	virtual void exitState();
	virtual int nextMode() { return 2; }
	virtual void getStats(float&, float&, int&);
private: 

	vector<HeavyDrone> enemies;
	vector<LightDrone> lightDrone;

	//vector<AmmoPickup> ammoPickup;
	//vector<HealthPickup> healthPickups;
	//vector<Objective> objectives;
	//vector<Interactable*> interactables;
	EntityHandler m_entityHandler;
	Level mLevel;

	PlayerController mPlayer;
	float playTime;
	bool win = false;
	bool lose = false;
	
	int curObj = 0;
	int oX, oY; //original window size
	bool isOver();
	bool objCompleted();
	void renderUI(float);
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;
};



#endif // !MainModeH

