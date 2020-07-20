#include "WindowUtils.h"
#include "D3D.h"
#include "FX.h"
#include "Mesh.h"
#include "Input.h"
#include "File.h"
#include "UserFolder.h"
#include "AudioMgrFMOD.h"
#include "Game.h"
#include "Enemy.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

Game gGame;
//Enemy enemy;

void Update(float dTime)
{
	gGame.Update(dTime);
	GetIAudioMgr()->Update();
	//enemy.Update(dTime);
}

void Render(float dTime)
{
	gGame.Render(dTime);
	//enemy.Render(dTime);
}

void OnResize(int screenWidth, int screenHeight)
{
	gGame.OnResize(screenWidth, screenHeight);
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INPUT:
		GetMouseAndKeys()->MessageEvent((HRAWINPUT)lParam);
		break;
	}

	return gGame.WindowsMssgHandler(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	SeedRandom();
	File::initialiseSystem();
	USER::SetUserDataFolder("MyGame");

	if (!InitMainWindow(1024, 768, hInstance, "Space Ace", MainWndProc))
		assert(false);

	if (!InitDirect3D(OnResize))
		assert(false);

	// Start up
	new FX::MyFX(gd3dDevice);
	new AudioMgrFMOD;
	GetIAudioMgr()->Initialise();

	new MeshManager;
	new MouseAndKeys;
	GetMouseAndKeys()->Initialise(GetMainWnd(), false, true);
	gGame.Initialise();

	Run(Update, Render);

	// Shut down
	gGame.Release();	// this function is currently empty. Sad face.
	delete GetMouseAndKeys();
	delete GetMeshManager();
	delete FX::GetMyFX();
	delete GetIAudioMgr();

	ReleaseD3D();

	return 0;
}
