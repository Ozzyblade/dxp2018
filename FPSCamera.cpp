#include "FPSCamera.h"
#include "D3D.h"
#include "D3DUtil.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


void FPSCamera::Initialise(const Vector3& pos, const DirectX::SimpleMath::Vector3& tgt, Matrix& viewSpaceTfm)
{
	mpViewSpaceTfm = &viewSpaceTfm;
	CreateViewMatrix(*mpViewSpaceTfm, pos, tgt, Vector3(0, 1, 0));
	mCamPos = pos;
}

void FPSCamera::Move(float dTime, float fb, float lr, bool shift)
{
	// Clamp values.
	float maxX = 300;	// Deliberately large so we shouldn't run into a horizontal clamp value.
	float maxZ = 300;	// Seems a sensible value for distance.


	if (fb == 0 && lr == 0)
		return;

	Matrix ori = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	/*Vector3 dir(0, 0, 1), up(0, 1, 0);
	dir = Vector3::TransformNormal(dir, ori);
	up = Vector3::TransformNormal(up, ori);
	Vector3 strafe = dir.Cross(up);*/
	Vector3 up(0, 1, 0);
	dir = Vector3(0, 0, 1);
	dir = Vector3::TransformNormal(dir, ori);
	up = Vector3::TransformNormal(up, ori);
	Vector3 strafe = dir.Cross(up);

	float actualSpeed;
	shift ? actualSpeed = speed * sprintMultiplier : actualSpeed = speed;

	Vector3 pos(mCamPos);

	bool isMoveable = false; 


	if (fb > 0)
	{
		//if (abs(pos.z < maxZ))
		pos += dir * dTime* actualSpeed * fb; //only sprint in forward dir not left right or back
	}
	else if (fb < 0)
	{
		actualSpeed *= 0.5;
		pos += dir * dTime* speed * fb;
	}

	if (lr > 0)
	{
		actualSpeed *= 0.75;
		pos += strafe * dTime * rspeed * lr;
		
	}
	else if (lr < 0)
	{
		actualSpeed *= 0.75;
		pos += strafe * dTime * rspeed * lr;
	}

	isMoveable = ptrLevel->LevelCollision(pos.x, pos.z);

	// Old Move Clamps.
	/*
	// Clamping movement;
	if (pos.z > maxZ)
		pos.z = maxZ;
	if (pos.z < -maxZ)
		pos.z = -maxZ;

	if (pos.x > maxX)
		pos.x = maxX;
	if (pos.x < -maxX)
		pos.x = -maxX;
	// End of Clamping Movement;
	*/
	
	// If movement is locked. Lock it. Are we using this for the Y Axis?
	if (mLockAxis.x != UNLOCK)
		pos.x = mLockAxis.x;
	if (mLockAxis.y != UNLOCK)
		//pos.y = mLockAxis.y;
		pos.y = -8.5;
	if (mLockAxis.z != UNLOCK)
		pos.z = mLockAxis.z;

	CreateViewMatrix(*mpViewSpaceTfm, pos, pos + dir, up);
	
	if (isMoveable)
		mCamPos = pos;
}
void FPSCamera::Rotate(float dTime, float _yaw, float _pitch, float _roll)
{
	yaw += _yaw * dTime * rspeed;
	if(!(pitch + _pitch * dTime * rspeed > 1.5 || pitch + _pitch * dTime * rspeed < -1.5)) //player cant roll around
	pitch += _pitch * dTime * rspeed;
	roll += _roll * dTime * rspeed;
	Matrix ori;
	ori = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	/*Vector3 dir(0, 0, 1), up(0, 1, 0);
	dir = Vector3::TransformNormal(dir, ori);
	up = Vector3::TransformNormal(up, ori);*/
	Vector3 up(0, 1, 0);
	dir = Vector3(0, 0, 1);
	dir = Vector3::TransformNormal(dir, ori);
	up = Vector3::TransformNormal(up, ori);
	CreateViewMatrix(*mpViewSpaceTfm, mCamPos, mCamPos + dir, up);
	mCamRot = dir;
}


