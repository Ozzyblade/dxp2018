#include "EndMode.h"

EndMode::EndMode()
{
	GetClientExtents(oX, oY);
}

EndMode::~EndMode()
{
	exitState();
}

void EndMode::update(float dTime)
{

}

void EndMode::render(float dTime)
{
	int w, h;
	GetClientExtents(w, h);
	//scaling
	float sX = (float)w / oX;
	float sY = (float)h / oY;
	string scores = highScr.highScoreTable();
	wstring enterName = L"ENTER NAME";
	wstring highScrTitle = L"HIGHSCORES";
	wstring hstbl = L"table";
	hstbl.assign(scores.begin(), scores.end());
	
	Vector2 enterNamePos(20 * sX, 60 * sY);
	Vector2 namePos(30 * sX, 100 * sY);
	Vector2 winLossPos((w / 2) - 100*sX, 20*sY);
	Vector2 highScorePos((w - 400) * sX, 60 * sY);
	Vector2 tablePos(w - 400*sX, 120 * sY);
	mpSpriteBatch->Begin(SpriteSortMode_Deferred);
	mpFont->DrawString(mpSpriteBatch, winLoss.c_str(), winLossPos, DirectX::Colors::Blue, 0, Vector2(0, 0), Vector2(1.95f*sX, 1.95f*sY));
	mpFont->DrawString(mpSpriteBatch, enterName.c_str(), enterNamePos, DirectX::Colors::Blue, 0, Vector2(0, 0), Vector2(1.95f*sX, 1.95f*sY));
	mpFont->DrawString(mpSpriteBatch, name.c_str(), namePos, DirectX::Colors::Blue, 0, Vector2(0, 0), Vector2(1.95f*sX, 1.95f*sY));
	mpFont->DrawString(mpSpriteBatch, highScrTitle.c_str(), highScorePos, DirectX::Colors::Blue, 0, Vector2(0, 0), Vector2(1.95f*sX, 1.95f*sY));
	mpFont->DrawString(mpSpriteBatch, hstbl .c_str(), tablePos, DirectX::Colors::Blue, 0, Vector2(0, 0), Vector2(1.95f*sX, 1.95f*sY));
	mpSpriteBatch->End();
}


void EndMode::enterState()
{
	modeOver = false;
	//load high scores
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	
	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/comicSansMS.spritefont");
}

void EndMode::exitState()
{
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}

void EndMode::getStats(float & score, float & time, int & status)
{
	if (status == 1)
	{
		playerScore = (score * 5/ (time * 0.2));
		winLoss = L"YOU WON!";
	}
	else
	{
		playerScore = 0;
		winLoss = L"YOU LOST!";
	}
}


void EndMode::handleKey(char letter)
{
	if (!nameEntered) {
		if (letter == '#' && name.size() > 0) //if backspace delete last char
		{
			name.pop_back();
			return;
		}
		else
		{
			if (letter == '@')
			{
				nameEntered = true; //enter key is hit
				string s(name.begin(), name.end());
				highScr.setUsername(s);
				highScr.setScore(playerScore);
				highScr.checkScoreAndReplace();
				
			}

			if (name.size() < 10 && letter != '#' && !nameEntered)
				name.push_back(letter);
		}
	}
}

void EndMode::loadScores()
{

}

