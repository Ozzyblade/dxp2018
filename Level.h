#ifndef LevelH
#define LevelH

#include <vector>

#include "Mesh.h"
#include "Model.h"		// 
#include "FPSCamera.h"	// acts as the player
#include "FX.h"
#include "GeometryBuilder.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
/*
	The Level will be made of blocks. Standard Geometry goes from -1 to 1 in all 3 axis. 
	If we're always going to be on the floor, we can change these values. 0 to 1.
	But for Sprint 1. Just render bits to make up 2 rooms.

	Map has to be Rectangular.

	Render 1 big floorplate and 1 ceiling plate. No need for a skybox.

	Map 0 = Empty
	Map 1 = Wall Block
	//
	Map 2 = horizontal Corridor block?
	Map 3 = Window Blocks
	Map 4 = Engine
	Map 5 = Monitor
	Map 6 = Wall Electronics
	Map 7 = Escape Pod
	Map 8 = Toolbox
*/


class Level {
public:

	static const int LevelX = 16;
	static const int LevelZ = 8;
	
	const float wallBuffer = 0.08f;
	const float corridorBuffer = 0.4f;

	// World Scaling value is 10; Magic Number set in Game.cpp currently.
	// Tried using a static int attribute of the level class but didn't work.

	// Why can't I use the LevelX and LevelZ values?
	// Because these values must be available at compile time. By making them static to the class, they are.

	// THE DIAGRAM BELOW IS CURRENTLY NORTH SOUTH INVERTED, BUT NOT EAST WEST INVERTED!

	/*
	const int Leveldesign[LevelZ][LevelX]{
	//int Leveldesign[4][8]{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,2,2,2,0,0,0,1,1,1,
	1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,
	1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
	1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};
	*/
	const int Leveldesign[LevelZ][LevelX]{
	//int Leveldesign[4][8]{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,1,1,0,0,1,7,2,5,5,1,0,0,1,
	1,0,0,2,2,0,0,1,1,1,0,0,2,0,4,1,
	1,5,0,1,1,0,0,1,1,1,0,0,1,1,1,1,
	1,1,1,1,1,0,0,2,2,2,0,0,1,1,1,1,
	1,0,0,2,2,0,6,1,1,1,0,0,2,2,8,1,
	1,0,6,1,1,1,1,1,1,1,1,1,1,1,0,1,
	1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
	};

	int GetMapWidth() { return LevelX; }
	int GetMapDepth() { return LevelZ; }

	bool LevelCollision(float& x, float& z) {

		// First we want to work out what cell we're in.
		// x Value / World Scaling (10).

		float newx = x* 0.1;
		float newz = z* 0.1;

		int xcell = (int)newx;
		int zcell = (int)newz;

		float localx = newx - xcell;
		float localz = newz - zcell;

		// If the values are outside of the designed level. This function should stop you moving there
		if (newx < 0 || newz < 0 || newx > LevelX || newz > LevelZ)
		{
			return false;
		}

		// convert a rough float position to a discreet int cell number.

		int whatCell = Leveldesign[zcell][xcell];

		if (whatCell == 1 || whatCell == 3)
			return false;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BUFFER SYSTEM //
		// Buffer Algorithm for Block Level Design - Put on hold because we're moving towards a single level model.
		// Buffer Aspect - What cells are around your current cell;
		int cellWest = (-1), cellEast = (-1), cellNorth = (-1), cellSouth = (-1);
		// X Buffers
		if (xcell > 0)	// If the player isn't in the west most cell...
		{
			cellWest = xcell - 1;
			cellWest = Leveldesign[zcell][cellWest];	// cellWest = what is in the cell to the west.
		}
		if (xcell < LevelX - 1)	//	 If the player isn't in the east most cell...
		{
			cellEast = xcell + 1;
			cellEast = Leveldesign[zcell][cellEast];	// cellEast = what is in the cell to the east.
		}
		// Z Buffers
		if (zcell > 0)
		{
			cellSouth = zcell - 1;
			cellSouth = Leveldesign[cellSouth][xcell];	// Remembering that North / South is inverted. 0 is the first line, but south position.
		}
		if (zcell < LevelZ - 1)
		{
			cellNorth = zcell + 1;
			cellNorth = Leveldesign[cellNorth][xcell];
		}
		// Now we have the cells, do we need to restrict movement?

		// Using Zones to show 
		//float eastbuffer = 0, westbuffer = 0, southbuffer = 0, northbuffer = 0;
		//float eastbufferH = 0, westbufferH = 0, southbufferH = 0, northbufferH = 0;

		//////////////////////////////////////////////////////////////
	


		switch (whatCell)
		{
		case 7:
		case 6:
		case 5:
		case 4:
		case 3:
		case 0: // Coords in a room. 
			// BUFFER CHECKS...
			// Check for wall buffers.
			if (cellEast == 1 || cellEast == 3)
			{
				if (localx > 1 - wallBuffer)
				{
					localx = 1 - wallBuffer;
					//return false;
				}
			}
			if (cellWest == 1 || cellWest == 3)
			{
				if (localx < wallBuffer)
				{
					localx = wallBuffer;
					//return false;
				}
			}
			if (cellSouth == 1 || cellSouth == 3)
			{
				if (localz < wallBuffer)
				{
					localz = wallBuffer;
					//return false;
				}
			}
			if (cellNorth == 1 || cellNorth == 3)
			{
				if (localz > 1 - wallBuffer)
				{
					localz = 1 - wallBuffer;
					//return false;
				}
			}
			
			// Corridor Buffer checks.
			if (cellEast == 2)
			{	// If you're too close to the east wall, but you're NOT right in front of the corridor. stop.
				if (localx > 1-wallBuffer && !(localz > corridorBuffer && localz < 1 - corridorBuffer))
					localx = 1 - wallBuffer;
					//return false;
			}
			if (cellWest == 2)
			{	// If you're too close to the west wall, as above.
				if (localx < wallBuffer && !(localz > corridorBuffer && localz < 1 - corridorBuffer))
					localx = wallBuffer;
					//return false;
			}
			/*
			*/
			
				

//			return true;
			break;
		case 1: // You're walking into a wall, you're not allowed. move along. 
			//	Buffer shouldn't let you walk through here, but could spawn here.
			return false;
			break;
		case 2: // Corridor. Now you need to check. What PART of the corridor are you trying to walk into?
			//z -= zcell; // 
			
			/*
			X value 147 when it comes in.
			* 0.1
			14.7
			x cell is 14.
			14.7 - 14 = 0.7 So you're .7 of the way along the corridor.
			All the X values are allowed in the corridor, it's just the Z value.
			So only check the Z value is less than 0.6 and more than 0.4
			*/
			if (localz < corridorBuffer)
				localz = corridorBuffer;
			
			if (localz > 1-corridorBuffer)
				localz = 1-corridorBuffer;
			break;
		//default: return false;
		}

		x = (localx + xcell) * 10;
		z = (localz + zcell) * 10;

		/*
		 Function Summary - I wrote this to explain what I'm thinking. BEFORE I wrote the function code.

		 We know (In theory) how wide each design cell of the level is, and where they are. (2D Array Above).
		 By using some maths, we can take the x and y input, and work out which cell that falls under.
		
		 If the player is trying to move inside a non-permissive cell, the function returns false.
		 The Player will not allow movement in an axis if not permitted by the Level's collision function.

		 This can cause issues with Gamepad movement...
		 ... Because moving at full speed towards a wall will stop them further away than walking up slowly would do.
		 Minor issue? Could be fixed in movement stage.
		*/
		return true;
	}
	
	void Update(float dTime, const int states[]);
	void Render(float dTime, int p_x, int p_z) {

		const int WORLDSCALING = 10;
		float tempScale;
		//get players xCoord and zCoord
		//only render 3 blocks on x and y
		//check if player is near x or z limit and set max or min to 0 or max val
	

		int startX = p_x - 5;
		int endX = p_x + 5;
		int startZ = p_z - 5;
		int endZ = p_z + 5;

		if (startX < 0) startX = 0;
		if (startZ < 0) startZ = 0;
		if (endX > LevelX) endX = LevelX;
		if (endZ > LevelZ) endZ = LevelZ;

		MaterialExt windowmat = mQuad.GetMesh().GetSubMesh(0).material;
		MaterialExt* pwindowmat = &windowmat;

		FX::GetMyFX()->Render(mroof, gd3dImmediateContext);
		FX::GetMyFX()->Render(mQuad, gd3dImmediateContext);
		for (int i(startZ); i < endZ; i++)
		{
			for (int j(startX); j < endX; j++)
			{
				switch (Leveldesign[i][j])
				{
				case 0: break;
				case 1: //	Wall block
					mBox.GetPosition() = Vector3(j*WORLDSCALING, -10, i*WORLDSCALING);
					FX::GetMyFX()->Render(mBox, gd3dImmediateContext);
					break;
				case 2:	//	East/West Corridor
					mCorridor.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2;
					mCorridor.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2;
					//mCorridor.GetRotation().y = PI / 2;
					FX::GetMyFX()->Render(mCorridor, gd3dImmediateContext);

					//mBox.GetPosition() = Vector3(0, 0, 0);
					tempScale = mBox.GetScale().z;	//	Store the original scare of the mBox
					mBox.GetScale().z *= 0.3f;
					mBox.GetPosition() = Vector3(j*WORLDSCALING, -10, i*WORLDSCALING);
					FX::GetMyFX()->Render(mBox, gd3dImmediateContext);
					mBox.GetPosition() = Vector3(j*WORLDSCALING, -10, (i*WORLDSCALING) + 7);
					FX::GetMyFX()->Render(mBox, gd3dImmediateContext);

					mBox.GetScale().z = tempScale;			//	Return to original without division
															//mBox.GetPosition() = Vector3(0, 0, 0);
															//mBox.GetScale() = Vector3(1.f, .5f, 1.f);

					break;
				case 3:	//	Render a Window Block.
					mBox.GetPosition() = Vector3(j*WORLDSCALING, -10, i*WORLDSCALING);
					FX::GetMyFX()->Render(mBox, gd3dImmediateContext);

					
					windowmat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
					windowmat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("SpaceWindow512.dds", true, gd3dDevice);
					windowmat.texture = "SpaceWindow512.dds";

					

					mWindow.GetPosition() = Vector3((j + 1)*WORLDSCALING, -13, i*WORLDSCALING);
					FX::GetMyFX()->Render(mWindow, gd3dImmediateContext, pwindowmat);
					break;
				case 4: // Render the Engine
					mEngine.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2 + 3.5;
					mEngine.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2 + 3.5;
					mEngine.GetScale() = Vector3(0.06, 0.06, 0.06);
					mEngine.GetRotation().y = PI / 2;
					FX::GetMyFX()->Render(mEngine, gd3dImmediateContext);

					break;
				case 5:	//	Roof  Monitor
					mMonitor.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2 + 1;
					mMonitor.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2 + 1;
					mMonitor.GetScale() = Vector3(0.02, 0.02, 0.02);
					FX::GetMyFX()->Render(mMonitor, gd3dImmediateContext);
					break;
				case 6:	//	Wall Electronics
					mWallE.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2 + 4;
					mWallE.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2 + 4;
					mWallE.GetScale() = Vector3(0.02, 0.02, 0.02);
					//mWallE.GetRotation().y = PI / 2;
					FX::GetMyFX()->Render(mWallE, gd3dImmediateContext);
					break;
				case 7:	//	Escape Pod
					/*
					*/
					mEscapePod.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2;
					mEscapePod.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2;
					mEscapePod.GetScale() = Vector3(0.02, 0.02, 0.02);
					//mWallE.GetRotation().y = PI / 2;
					FX::GetMyFX()->Render(mEscapePod, gd3dImmediateContext);
					break;
				case 8:	//	Toolbox
					mToolbox.GetPosition().x = j * WORLDSCALING + WORLDSCALING / 2;
					mToolbox.GetPosition().z = i * WORLDSCALING + WORLDSCALING / 2;
					mToolbox.GetScale() = Vector3(0.2, 0.3, 0.2);
					mToolbox.GetRotation().y = -PI / 3;
					FX::GetMyFX()->Render(mToolbox, gd3dImmediateContext);
				}

			}
		}

	};


	void Initalise()
	{
		mQuad.Initialise(*GetMeshManager()->GetMesh("quad"));
		//textured lit box
		mBox.Initialise(*GetMeshManager()->GetMesh("cube"));
		
		// Build Size is 1 x 1 x 1.
		//mBox.GetPosition() = Vector3(0, -0.5f, 1);
		mBox.GetScale() = Vector3(10.f, 5.f, 10.f);
		mBox.GetPosition() = Vector3(0, 0, 1);
		//mBox.GetScale() = Vector3(0.5f, 0.5f, 0.5f);
		MaterialExt mat = mQuad.GetMesh().GetSubMesh(0).material;
		mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
		// DON'T USE, NOT RENDERED THIS WAY FOR WINDOWS. MATT DID THIS.
		mat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("SpaceWall1.dds", true, gd3dDevice);
		mat.texture = "SpaceWall1.dds";
		mBox.SetOverrideMat(&mat);

		mWindow.Initialise((*GetMeshManager()->GetMesh("window")));
		// Window
		mWindow.GetScale() = Vector3(10, 10, 10);
		mWindow.GetRotation() = Vector3(0, -PI / 2, 0);
		mat = mWindow.GetMesh().GetSubMesh(0).material;
		//mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
		mat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("SpaceWindow512.dds", true, gd3dDevice);
		mat.texture = "SpaceWindow512.dds";
		mat.texTrsfm.scale = Vector2(10, 10);
		mWindow.SetOverrideMat(&mat);

		// Corridor model
		Mesh& corridorMesh = *GetMeshManager()->GetMesh("cormesh");
		corridorMesh.CreateFrom("data/R6_BridgeWindowed.fbx", gd3dDevice, FX::GetMyFX()->mCache);
		mCorridor.Initialise(corridorMesh);
		mCorridor.GetScale() = Vector3(5.f, 1.f, 0.5f);
		mCorridor.GetPosition() = Vector3(0, -8.32, 0);
		mCorridor.GetRotation() = Vector3(0, 0, 0);
		MaterialExt& corMat = mCorridor.GetMesh().GetSubMesh(0).material;
		//coinMat.flags | MaterialExt::LIT;
		//coinMat.flags &= ~MaterialExt::LIT;
		//corMat.flags &= ~MaterialExt::ZTEST;
		corMat.flags &= ~MaterialExt::CULL;

		// floor
		mQuad.GetScale() = Vector3(200, 10, 200);
		mQuad.GetPosition() = Vector3(0, -10, 0);
		mat = mQuad.GetMesh().GetSubMesh(0).material;
		mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
		mat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("Floor1.dds", true, gd3dDevice);
		mat.texture = "Floor1.dds";
		mat.texTrsfm.scale = Vector2(10, 10);
		mQuad.SetOverrideMat(&mat);


		mroof.Initialise(mQuad.GetMesh());
		mroof.GetScale() = Vector3(200, 10, 200);
		mroof.GetPosition() = Vector3(0, -5.f, 0);
		mroof.GetRotation() = Vector3(0, 0, PI);

		mroof.SetOverrideMat(&mat);

		// Engine
		Mesh& engMesh = *GetMeshManager()->GetMesh("engmesh");
		mEngine.Initialise(engMesh);

		

		//mEngine.GetScale() = Vector3(5.f, 1.f, 0.5f);
		mEngine.GetPosition() = Vector3(0, -8.92, 0);
		mEngine.GetRotation() = Vector3(0, 0, 0);

		// Monitor
		Mesh& monMesh = *GetMeshManager()->GetMesh("monmesh");
		mMonitor.Initialise(monMesh);

		MaterialExt monText = monMesh.GetSubMesh(0).material;
		monText.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("Tex1.dds", true, gd3dDevice);
		monText.texture = "Tex1.dds";
		mMonitor.SetOverrideMat(&monText);

		//mMonitor.GetScale() = Vector3(5.f, 1.f, 0.5f);
		mMonitor.GetPosition() = Vector3(0, -5.32, 0);
		mMonitor.GetRotation() = Vector3(0, 0, 0);

		// Wall Electronics
		Mesh& wallEMesh = *GetMeshManager()->GetMesh("wallEmesh");
		mWallE.Initialise(wallEMesh);

		MaterialExt wallText = wallEMesh.GetSubMesh(0).material;
		wallText.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("Tex1.dds", true, gd3dDevice);
		wallText.texture = "Tex1.dds";
		mWallE.SetOverrideMat(&wallText);

		mWallE.GetPosition() = Vector3(0, -8.32, 0);


		// Escape Pod
		Mesh& EscMesh = *GetMeshManager()->GetMesh("escmesh");
		mEscapePod.Initialise(EscMesh);
		mEscapePod.GetPosition() = Vector3(0, -8.32, 0);

		// Toolbox
		Mesh& toolMesh = *GetMeshManager()->GetMesh("toolmesh");
		mToolbox.Initialise(toolMesh);

		MaterialExt toolTx = toolMesh.GetSubMesh(0).material;
		toolTx.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("Tex1.dds", true, gd3dDevice);
		toolTx.texture = "Tex1.dds";
		mToolbox.SetOverrideMat(&toolTx);

		mToolbox.GetPosition() = Vector3(0, -9.62, 0);

	};
	void Release();

	
	Model mWindow;
	Model mBox;
	Model mQuad;
	Model mCorridor;
	Model mroof;
	Model mEngine;		//	Engine
	Model mMonitor;		//	Roof Monitor
	Model mWallE;		//	Wall Electronics
	Model mEscapePod;	//	Escape Pod
	Model mToolbox;		//	Toolbox

private:
	
};
#endif // LevelH