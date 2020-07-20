#include <algorithm>

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "FX.h"
#include "Input.h"
#include "Level.h"
#include "Interactable.h"
#include "HeavyDrone.h"
#include "AudioMgrFMOD.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

Vector3 gWorldScale(10, 10, 10);
Vector3 PistolOffset(0.3, -0.2, 0.7);
//Vector3 PistolOffsetR(0, 0, 0);

//Audio
float musicvolume = 0.4;			//	All sounds volume I think. Value between 0 and 1.
unsigned int musicHdl = 1;	//	Handle to the Background Music. 1: New York.
//

// This value is used to prevent the corridor being rendered partly beneath the floor panel.
float corridorHeight = -.832;

// This will set the Initial Camera Height. But the Height of the camera is set inside FPSCamera.cpp
// Move Function - Line circa 75. 
float cameraHeight = -8.5;

// Why can't I declare these in the Header file??
DirectX::SpriteBatch *mpSpriteBatch = nullptr;
DirectX::SpriteFont *mpFont = nullptr;

//HeavyDrone test1;

bool torchIsActive = true;

void Game::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Game::Initialise()
{
	mQuad.Initialise(BuildQuad(*GetMeshManager()));

	//textured lit box
	mBox.Initialise(BuildCube(*GetMeshManager()));
	mWindow.Initialise(BuildCube(*GetMeshManager(),1));
	MaterialExt mat = mQuad.GetMesh().GetSubMesh(0).material;
	
	#pragma region HeavyDrone Mesh
	// Enemy Drone Initialisation
	Mesh& body = GetMeshManager()->CreateMesh("drone04_body");
	body.CreateFrom("data/drone04_body.FBX",gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& gunl = GetMeshManager()->CreateMesh("drone04_gun_L");
	gunl.CreateFrom("data/drone04_gun_L.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& gunr = GetMeshManager()->CreateMesh("drone04_gun_R");
	gunr.CreateFrom("data/drone04_gun_R.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	#pragma endregion

	#pragma region LightDrone Mesh
	Mesh& lightDrone_body = GetMeshManager()->CreateMesh("drone02_body");
	lightDrone_body.CreateFrom("data/drone02_body.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& lightDrone_left = GetMeshManager()->CreateMesh("drone02_left");
	lightDrone_left.CreateFrom("data/drone02_left.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& lightDrone_right = GetMeshManager()->CreateMesh("drone02_right");
	lightDrone_right.CreateFrom("data/drone02_right.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& lightDrone_left_g = GetMeshManager()->CreateMesh("drone02_left_g");
	lightDrone_left_g.CreateFrom("data/drone02_left_g.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& lightDrone_right_g = GetMeshManager()->CreateMesh("drone02_right_g");
	lightDrone_right_g.CreateFrom("data/drone02_right_g.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	#pragma endregion

	#pragma region Icons
	Mesh& icon_Ammo = GetMeshManager()->CreateMesh("icon_Ammo");
	icon_Ammo.CreateFrom("data/SM_Icon_Ammo.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& icon_Health = GetMeshManager()->CreateMesh("icon_Health");
	icon_Health.CreateFrom("data/SM_Icon_Health.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	Mesh& icon_Spanner = GetMeshManager()->CreateMesh("icon_Spanner");
	icon_Spanner.CreateFrom("data/SM_Icon_Spanners.FBX", gd3dDevice, FX::GetMyFX()->mCache);
	#pragma endregion


	// floor
	mQuad.GetScale() = Vector3(20, 1, 20);
	mQuad.GetPosition() = Vector3(0, -1, 0);
	mat = mQuad.GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("Floor1.dds", true, gd3dDevice);
	mat.texture = "Floor1.dds";
	mat.texTrsfm.scale = Vector2(10, 10);
	mQuad.SetOverrideMat(&mat);


	// Roof (Copy of Floor)q
	mroof.Initialise(mQuad.GetMesh());
	mroof.GetScale() = Vector3(20, 1, 20);
	mroof.GetPosition() = Vector3(0, -0.5f, 0);
	mroof.GetRotation() = Vector3(0, 0, PI);
	mroof.SetOverrideMat(&mat);

	// Window
	mWindow.GetScale() = Vector3(10, 10, 10);
	mWindow.GetRotation() = Vector3(0, -PI / 2,0);
	mat = mWindow.GetMesh().GetSubMesh(0).material;
	mat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("SpaceWindow512.dds", true, gd3dDevice);
	mat.texture = "SpaceWindow512.dds";
	mat.texTrsfm.scale = Vector2(10, 10);
	mWindow.SetOverrideMat(&mat);

	// Pistol model
	Mesh& pistolMesh = GetMeshManager()->CreateMesh("pistolmesh");
	pistolMesh.CreateFrom("data/heavy.FBX", gd3dDevice, FX::GetMyFX()->mCache);

	Mesh& bulletTest = GetMeshManager()->CreateMesh("bullet");
	bulletTest.CreateFrom("data/b8.FBX", gd3dDevice, FX::GetMyFX()->mCache);

	// Corridor model
	Mesh& corridorMesh = GetMeshManager()->CreateMesh("cormesh");
	corridorMesh.CreateFrom("data/R6_BridgeWindowed.fbx", gd3dDevice, FX::GetMyFX()->mCache);
	mCorridor.Initialise(corridorMesh);
	mCorridor.GetScale() = Vector3(0.5f, 0.1f, 0.05f);
	mCorridor.GetPosition() = Vector3(0, corridorHeight, 0);
	mCorridor.GetRotation() = Vector3(0, 0, 0);
	MaterialExt& corMat = mCorridor.GetMesh().GetSubMesh(0).material;
	corMat.flags &= ~MaterialExt::CULL;

	// Engine Mesh
	Mesh& engMesh = GetMeshManager()->CreateMesh("engmesh");
	engMesh.CreateFrom("data/Engine1.obj", gd3dDevice, FX::GetMyFX()->mCache);
	mEng.Initialise(engMesh);

	// Monitor Mesh
	Mesh& monMesh = GetMeshManager()->CreateMesh("monmesh");
	monMesh.CreateFrom("data/SM_Prop_Security_Monitor_01.fbx", gd3dDevice, FX::GetMyFX()->mCache);
	//monMesh.CreateFrom("data/R6_BridgeWindowed.fbx", gd3dDevice, FX::GetMyFX()->mCache);
	mMonitor.Initialise(monMesh);

	// Wall Electronics
	Mesh& wallEMesh = GetMeshManager()->CreateMesh("wallEmesh");
	wallEMesh.CreateFrom("data/SM_Prop_WallAttach_Electronics_01.fbx", gd3dDevice, FX::GetMyFX()->mCache);
	mWallE.Initialise(wallEMesh);

	// Escape Pod
	Mesh& ePodMesh = GetMeshManager()->CreateMesh("escmesh");
	ePodMesh.CreateFrom("data/HoverCar.obj", gd3dDevice, FX::GetMyFX()->mCache);
	mEscapePod.Initialise(ePodMesh);

	// Toolbox
	Mesh& toolMesh = GetMeshManager()->CreateMesh("toolmesh");
	toolMesh.CreateFrom("data/SM_Wep_Ammo_Battery_01.fbx", gd3dDevice, FX::GetMyFX()->mCache);
	mToolbox.Initialise(toolMesh);


	

	//scale the world
	mOpaques.push_back(&mQuad);
	mOpaques.push_back(&mroof);
	mOpaques.push_back(&mBox);
	mOpaques.push_back(&mCorridor);
	
	for (Model* obj : mOpaques)
	{
		obj->GetScale() *= gWorldScale;
		obj->GetPosition() *= gWorldScale;
	}

	curMode = 0;
	modes.push_back(new StartMode);
	modes.push_back(new MainMode);
	modes.push_back(new EndMode);
	FX::SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.25f, 0.25f, 0.25f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.1, 0.1, 0.1));
	// Add an additional light. But how do we change these dynamically?
	FX::SetupPointLight(1, true, Vector3(15, -8, 0), Vector3(1, 0, 0));
	//i.model.GetPosition() = Vector3(15, 0, 15);
	Vector3 mCameraStartPos = Vector3(15, cameraHeight, 12);
	
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/comicSansMS.spritefont");
	
	modes[curMode]->enterState();
}


void Game::Release()
{
	//delete the gameModes we created
	for (int i = 0; i < modes.size(); i++)
	{
		delete modes[i];
		modes.erase(modes.begin() + i);
		i--;
	}
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}



void Game::Update(float dTime)
{
	
	modes[curMode]->update(dTime);

	//check if current mode is over
	if (modes[curMode]->endMode())
	{
		modes[curMode]->exitState();
		//ask mode thats just ended what is the next mode we want to go to
		curMode = modes[curMode]->nextMode();
		if(curMode != 2)
			modes[curMode]->enterState();
		else
		{
			//pass over score time and player status to end mode
			modes[curMode]->enterState();
			float s, t;
			int i;
			modes[1]->getStats(s, t, i);
			modes[curMode]->getStats(s, t, i);
		}
	}

	if (GetIAudioMgr()->GetSongMgr()->IsPlaying(musicHdl) == false)	
		GetIAudioMgr()->GetSongMgr()->Play("DoomOST", true, false, &musicHdl, musicvolume);

}

void Game::Render(float dTime)
{
	BeginRender(Colours::Black);


	CreateProjectionMatrix(FX::GetProjectionMatrix(), 0.25f*PI, GetAspectRatio(), 0.1f, 1000.f);
	
	modes[curMode]->render(dTime);
	EndRender();

	GetMouseAndKeys()->PostProcess();
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 200.f * GetElapsedSec();

	switch (msg)
	{
	case WM_INPUT:
		GetMouseAndKeys()->MessageEvent((HRAWINPUT)lParam);
		break;
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
			PostQuitMessage(0);
			return 0;
		case 'a':	modes[curMode]->handleKey('a');	break;
		case 'b':	modes[curMode]->handleKey('b');	break;
		case 'c':	modes[curMode]->handleKey('c');	break;
		case 'd':	modes[curMode]->handleKey('d');	break;
		case 'e':	modes[curMode]->handleKey('e');	break;
		case 'f':	modes[curMode]->handleKey('f');	break;
		case 'g':	modes[curMode]->handleKey('g');	break;
		case 'h':	modes[curMode]->handleKey('h');	break;
		case 'i':	modes[curMode]->handleKey('i');	break;
		case 'j':	modes[curMode]->handleKey('j');	break;
		case 'k':	modes[curMode]->handleKey('k');	break;
		case 'l':	modes[curMode]->handleKey('l');	break;
		case 'm':	modes[curMode]->handleKey('m');	break;
		case 'n':	modes[curMode]->handleKey('n');	break;
		case 'o':	modes[curMode]->handleKey('o');	break;
		case 'p':	modes[curMode]->handleKey('p');	break;
		case 'q':	modes[curMode]->handleKey('q');	break;
		case 'r':	modes[curMode]->handleKey('r');	break;
		case 's':	modes[curMode]->handleKey('s');	break;
		case 't':	modes[curMode]->handleKey('t');	break;
		case 'u':	modes[curMode]->handleKey('u');	break;
		case 'v':	modes[curMode]->handleKey('v');	break;
		case 'w':	modes[curMode]->handleKey('w');	break;
		case 'x':	modes[curMode]->handleKey('x');	break;
		case 'y':	modes[curMode]->handleKey('y');	break;
		case 'z':	modes[curMode]->handleKey('z');	break;
		case '1':	modes[curMode]->handleKey('1');	break;
		case '2':	modes[curMode]->handleKey('2');	break;
		case '3':	modes[curMode]->handleKey('3');	break;
		case '4':	modes[curMode]->handleKey('4');	break;
		case '5':	modes[curMode]->handleKey('5');	break;
		case '6':	modes[curMode]->handleKey('6');	break;
		case '7':	modes[curMode]->handleKey('7');	break;
		case '8':	modes[curMode]->handleKey('8');	break;
		case '9':	modes[curMode]->handleKey('9');	break;
		case '0':	modes[curMode]->handleKey('0');	break;
		case '!':	modes[curMode]->handleKey('!');	break;
		case '$':	modes[curMode]->handleKey('$');	break;
		case 8:     modes[curMode]->handleKey('#'); break; //backspace and enter
		case 13:    modes[curMode]->handleKey('@'); break;
		}
	}

	//default message handling (resize window, full screen, etc)
	return DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

