#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "FPSCamera.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "bullet.h"
#include <vector>
#include "MainMode.h"
#include "StartMode.h"
#include "EndMode.h"
#include "gameMode.h"
class Game
{
public:

	Game() {}
	~Game() {
		Release();
	}
	void Update(float dTime);
	void Render(float dTime);
	void OnResize(int screenWidth, int screenHeight);
	void Initialise();
	void Release();
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	Model mBox, mQuad, mCorridor, mroof, mEnemyDemo, mWindow, mEng, mMonitor, mWallE, mEscapePod, mToolbox;

private:
	Game& operator=(const Game&) = delete;
	Game(const Game& m) = delete;
	//Level l;
	gameMode* whatModeAmIRunning = nullptr;
	int curMode;
	vector<gameMode*> modes;
	float gAngle = 0;
	//Enemy enemy;
	PlayerController mpController;

	
	std::vector<Model*> mOpaques;
};

#endif

