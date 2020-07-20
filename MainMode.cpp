#include "MainMode.h"
#include "Mesh.h"

MainMode::MainMode()
{
	modeOver = false;
	GetClientExtents(oX, oY);
}

MainMode::~MainMode()
{
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}

void MainMode::update(float dTime)
{ 
	if (!isOver())
	{
		playTime += dTime;
		mPlayer.Update(dTime);
		m_entityHandler.Update(dTime);
		}
	else
	{
		modeOver = true;
	}
}

void MainMode::render(float dTime)
{
	FX::SetPerFrameConsts(gd3dImmediateContext, mPlayer.GetPos());
	mLevel.Render(dTime, mPlayer.coordX, mPlayer.coordZ);

	m_entityHandler.Render();
	
	mPlayer.Render(dTime);
	renderUI(dTime);
}

void MainMode::enterState()
{

	mPlayer.ptrLevel = &mLevel;
	mPlayer.initialise();
	/*HeavyDrone h;
	h.Initalise(&mPlayer, 0.5, Vector3(1, 0, 1), Vector3(20, -8.5, 80));
	enemies.push_back(h);
	HeavyDrone h1(h);
	h1.Initalise(&mPlayer, 0.5, Vector3(1, 0, 1), Vector3(60, -8.5, 35));
	enemies.push_back(h1);*/


	
	mLevel.Initalise();

	

	playTime = 0;

	m_entityHandler.Initialise(&mPlayer);

	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/comicSansMS.spritefont");
}

void MainMode::exitState()
{
}

void MainMode::getStats(float & score, float & time, int & status)
{
	score = mPlayer.score;
	time = playTime;
	if (mPlayer.isAlive())
		status = 1;
	else
		status = 0;
}


bool MainMode::isOver()
{
	//if player dies or completes objective end game
	if (mPlayer.isAlive() && !objCompleted())
		return false;
	if (mPlayer.isAlive())
		win = true;
	else
		lose = true;
	return true;
}

bool MainMode::objCompleted()
{
	vector<Objective*> objs = m_entityHandler.getObjectives();
	
	for (int i = 0; i < objs.size(); ++i)
		if (objs[i]->GetActive()) 
			return false;
	return true;
}

void MainMode::renderUI(float dTime)
{

	wstring objmsg = L"REPAIR ALL SHIP SYSTEMS";

	wstring ammomsg = L"Ammo: " + to_wstring(mPlayer.ammo) + L"/" + to_wstring(mPlayer.maxAmmo);
	wstring magmsg = L"Mags: " + to_wstring(mPlayer.mag) + L"/" + to_wstring(mPlayer.magMax);
	wstring healthmsg = L"Health: " + to_wstring(mPlayer.health) + L"/" + to_wstring(mPlayer.healthMax);
	wstring scoremsg = L"Score: " + to_wstring(mPlayer.score);

	wstring interactmsg = L"Press E to interact";
	wstring interactmsg2 = L"Press B to interact";

	wstring fps = L"FPS " + to_wstring(1.f / dTime);
	
	wstring twin = L"YOU WIN WITH A SCORE OF " + to_wstring(mPlayer.score);
	wstring tlose = L"YOU LOSE!";
	wstring leftSitckHelpers = L"X: " + to_wstring(mPlayer.GetControllerX()) + L" Y: " + to_wstring(mPlayer.GetControllerY());
	wstring objCounter1 = L"ObjCount: " + to_wstring(m_entityHandler.GetObjectiveCount());
	int w, h;
	GetClientExtents(w, h);
	// Positions for Images
	float sY = (float)h / oY;
	float sX = (float)w / oX;
	Vector2 reticlepos(w / 2 - 26, h / 2 - 26);
	Vector2 uipos(0, 635*sY);

	DirectX::CommonStates reticleAlphaTransparency(gd3dDevice);
	mpSpriteBatch->Begin(SpriteSortMode_Deferred, reticleAlphaTransparency.NonPremultiplied());
	mpSpriteBatch->Draw(FX::GetMyFX()->mCache.LoadTexture("UIWindow.dds", true, gd3dDevice), uipos, nullptr, Colours::White, 0, Vector2(0, 0), Vector2(8*sX, 1*sY));
	mpFont->DrawString(mpSpriteBatch, ammomsg.c_str(), Vector2(10*sX, 660*sY), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f*sX, 0.75f*sY));
	mpFont->DrawString(mpSpriteBatch, magmsg.c_str(), Vector2(10*sX, 680*sY), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f*sX, 0.75f*sY));
	mpFont->DrawString(mpSpriteBatch, healthmsg.c_str(), Vector2(10*sX, 700*sY), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f*sX, 0.75f*sY));
	mpFont->DrawString(mpSpriteBatch, scoremsg.c_str(), Vector2(1024 / 2 - 50*sX, 40*sY), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f*sX, 0.75f*sY));
	mpSpriteBatch->Draw(FX::GetMyFX()->mCache.LoadTexture("Reticle.dds", true, gd3dDevice), reticlepos, nullptr, Colours::White, 0, Vector2(0, 0), Vector2(0.1f, 0.1f));
	// Debug Message

	mpFont->DrawString(mpSpriteBatch, objCounter1.c_str(), Vector2(100, 320), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));

	if(win)
		mpFont->DrawString(mpSpriteBatch, twin.c_str(),reticlepos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));
	if(lose)
		mpFont->DrawString(mpSpriteBatch, tlose.c_str(), reticlepos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));





	//wstring objmsg;
	switch (mPlayer.score)
	{
	//case 0: objmsg = objmssg1; break;
	//case 50: objmsg = objmssg2; break;
	}

	if (m_entityHandler.GetEntityUIState() != 0) {
		if (m_entityHandler.GetEntityUIState() == 1)
			mpFont->DrawString(mpSpriteBatch, interactmsg.c_str(), Vector2(512 - 200, 360), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(2.0f, 2.0f));
		else
			if (m_entityHandler.GetEntityUIState() == 2)
				mpFont->DrawString(mpSpriteBatch, interactmsg2.c_str(), Vector2(512 - 200, 360), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(2.0f, 2.0f));
	}

	mpFont->DrawString(mpSpriteBatch, objmsg.c_str(), Vector2(w/2, 680*sY), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(1, 1));

	mpSpriteBatch->End();

}
