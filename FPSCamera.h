#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include "d3d11.h"
#include "Level.h"
#include "SimpleMath.h"

/*
First person camera, forwards backwards left right and rotate
*/
class FPSCamera
{

public:
	enum { UNLOCK = 999999 };
	Level* ptrLevel = nullptr;

	//set initial camera position and the matrix to modify
	void Initialise(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& tgt, DirectX::SimpleMath::Matrix& viewSpaceTfm);

	//movement can be driven by WASD
	void Move(float dTime, float forwardBack, float leftRight, bool shift = false);
	//rotation can be driven by mouse x=yaw and y=pitch
	void Rotate(float dTime, float _yaw, float _pitch, float _roll);
	//stop camera moving in a certain axis, e.g. a FPS camera that always stays a fixed height in Y
	void LockMovementAxis(float x = UNLOCK, float y = UNLOCK, float z = UNLOCK) {
		mLockAxis = DirectX::SimpleMath::Vector3(x, y, z);
	}
	const DirectX::SimpleMath::Vector3& GetPos() {
		return mCamPos;
	}

	const DirectX::SimpleMath::Vector3& GetRot() {
		return mCamRot;
	}
	
	
const DirectX::SimpleMath::Matrix* getMatrix()
	{
		//(assert mpViewSpaceTfm);
		return mpViewSpaceTfm;
	}
	

	
	float yaw = 0, pitch = 0, roll = 0;

	DirectX::SimpleMath::Vector3 dir;
private:
	DirectX::SimpleMath::Vector3 mLockAxis{ UNLOCK,UNLOCK,UNLOCK };
	//the camera matrix to modify
	DirectX::SimpleMath::Matrix* mpViewSpaceTfm;
	//movement and rotation speed
	float speed = 6.f, rspeed = 3.f; // rSpeed is Rotate Speed or "Look speed".
	float sprintMultiplier = 1.5;
	//camera position
	DirectX::SimpleMath::Vector3 mCamPos;
	DirectX::SimpleMath::Vector3 mCamRot;
	//camera rotation
	
};


#endif
