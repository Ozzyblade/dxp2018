#include "StartMode.h"

StartMode::StartMode()
{
	modeOver = false;
}

StartMode::~StartMode()
{
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}

void StartMode::update(float dTime)
{
	// Arbitrary stuff for test purposes. Probably delete me, or use me to animate a symbol.
	float multiplier = 5;
	if (changingNumber < 100)
	changingNumber += dTime * multiplier;
	else changingNumber = 0;

	//if (GetMouseAndKeys()->IsPressed(VK_LSHIFT))
	//		modeOver = true;

	// Only allow further input if the user has stopped pressing both Up and Down keys.
	if (!GetMouseAndKeys()->IsPressed(VK_S) && !GetMouseAndKeys()->IsPressed(VK_W))
		menuBounce = false;
	
	// If they are pressing either the Up or Down keys, only act if allowed to. Otherwise do nothing.
	else if (!menuBounce)
	{
		bool sound = false;
		if (GetMouseAndKeys()->IsPressed(VK_W) && !GetMouseAndKeys()->IsPressed(VK_S) && menuChoice > 1)
		{
			menuChoice = 1;
			sound = true;
		}
		else if (GetMouseAndKeys()->IsPressed(VK_S) && !GetMouseAndKeys()->IsPressed(VK_W) && menuChoice < 4)
		{
			menuChoice = 4;
			sound = true;
		}

		if (sound)
		{
			menuBounce = true;
			GetIAudioMgr()->GetSfxMgr()->Play("Click", false, false, nullptr, 1);
		}
	}

	if (GetMouseAndKeys()->IsPressed(VK_RETURN))
	{
		// Each case is a menu choice, pointing to a different Mode (Except Exit, that just quits).
		switch (menuChoice)
		{

		case 1: 
		case 2: 
		case 3:
			modeOver = true; break;
		case 4: PostQuitMessage(0);  break;
		}
	}
}

void StartMode::render(float dTime)
{
	//FX::SetPerFrameConsts(gd3dImmediateContext, mPlayer.GetPos());
	bool win = false, lose = false;

	int w, h;
	GetClientExtents(w, h);
	// Positions for Images
	Vector2 reticlepos(w / 2 - changingNumber, h / 2 - changingNumber);
	Vector2 uipos(0, h - 128 - 5);

	Vector2 option1Pos(150, h / 2 - 100);
	Vector2 option2Pos(180, h / 2 - 50);
	Vector2 option3Pos(210, h / 2);
	Vector2 option4Pos(w/2, h / 2 + 250);
	Vector2 selectPos;

	wstring title = L"Space Ace";
	wstring opt1 = L"New Game";
	wstring opt2 = L"WASD to move, Shift to sprint, E to interact, F for flashlight";
	wstring opt3 = L"Left Click to Shoot, Right Click to aim down sights";
	wstring opt4 = L"Exit Game";

	DirectX::CommonStates reticleAlphaTransparency(gd3dDevice);
	mpSpriteBatch->Begin(SpriteSortMode_Deferred, reticleAlphaTransparency.NonPremultiplied());
	mpFont->DrawString(mpSpriteBatch, title.c_str(), Vector2(w/2 - 180, 100), DirectX::Colors::Blue, -0.1, Vector2(0, 0), Vector2(1.95f, 1.95f));
	mpFont->DrawString(mpSpriteBatch, opt1.c_str(), option1Pos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));
	mpFont->DrawString(mpSpriteBatch, opt2.c_str(), option2Pos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));
	mpFont->DrawString(mpSpriteBatch, opt3.c_str(), option3Pos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));
	mpFont->DrawString(mpSpriteBatch, opt4.c_str(), option4Pos, DirectX::Colors::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));
	
	switch (menuChoice)
	{
	case 1: selectPos = option1Pos; break;
	case 2: selectPos = option2Pos; break;
	case 3: selectPos = option3Pos; break;
	case 4: selectPos = option4Pos; break;
	}
	selectPos.x -= 55;
	selectPos.y -= 15;
	
	mpSpriteBatch->Draw(FX::GetMyFX()->mCache.LoadTexture("Reticle.dds", true, gd3dDevice), selectPos, nullptr, Colours::White, 0, Vector2(0, 0), Vector2(0.1f, 0.1f));
	mpSpriteBatch->End();
}

void StartMode::enterState() {
	// What do you do when you enter this state?
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/comicSansMS.spritefont");
}

void StartMode::exitState() {
	// What do you delete when you leave?
}