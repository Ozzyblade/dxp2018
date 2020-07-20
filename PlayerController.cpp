#include "PlayerController.h"
#include "D3DUtil.h"
#include "FX.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayerController::PlayerController()
	: ptrLevel(nullptr), interactDown(false), bSpeed(32)
{
	mPad.Initialise();
	fireMode = AUTO;
	m_MouseLastInput = true;
}

PlayerController::~PlayerController()
{
	for (int i = 0; i < pBullets.size(); i++)
	{
		delete(pBullets[i]);
		pBullets.erase(pBullets.begin() + i);
		i--;
	}
}

void PlayerController::Update(float dTime)
{

	FX::SetupPointLight(2, muzzleFlash, ptrCam.GetPos(), ptrCam.GetRot(), Vector3(1, 1, 0), Vector3(0, 0, 0), 30.f, 1.0f);
	tTime += dTime; // tTime is the total time elasped
	handleInputs(dTime);
	m_bobTimer += dTime;
	HandleWeaponBob(dTime);
	RecoilHandle(dTime);
	coordX = GetPos().x / 10;
	coordZ = GetPos().z / 10;
}

void PlayerController::HandleWeaponBob(float dTime) {
	m_waveSlice = 0.0;

	if (m_inputType == 1) {
		m_hor = mPad.GetState(0).leftStickX;
		m_vert = mPad.GetState(0).leftStickY;
	}

	if (m_inputType == 0) {
		if (m_pressed) {
			m_hor = 1;
			m_vert = 1;
		}
		else {
			m_hor = 0;
			m_vert = 0;
		}
	}

	if (abs(m_hor) == 0 && abs(m_vert) == 0) {
		m_bobTimer = 0;
	}
	else {
		m_waveSlice = sin(m_bobTimer);
		m_bobTimer = m_bobTimer + m_bobSpd;

		if (m_bobTimer > PI * 2) {
			m_bobTimer = m_bobTimer - (PI * 2);
		}
	}

	if (m_waveSlice != 0) {
		m_translateChange = m_waveSlice * m_bobAmt;
		m_totAxes = abs(m_hor) + abs(m_vert);
		m_totAxes = clamp(m_totAxes, 0.0, 1.0);
		m_translateChange = m_totAxes * m_translateChange;
		cGunPos.y = (m_translateChange - m_bobOffset);
	}
}

float PlayerController::clamp(float n, float lower, float upper) {
	return max(lower, min(n, upper));
}

void PlayerController::Render(float dTime)
{

	Matrix camLocalToWorld = *ptrCam.getMatrix();
	camLocalToWorld = camLocalToWorld.Invert();

	// mGun.GetRotation() = Vector3(0, PI, 0);	// or 2 PI over 4. to get 90 degrees. To turn the gun model facing "relative forward"
	mGun.GetScale() = Vector3(0.08, 0.08, 0.08);
	mGun.GetPosition() = cGunPos;

	Quaternion def = XMQuaternionRotationRollPitchYaw(mGun.GetRotation().x, mGun.GetRotation().y, mGun.GetRotation().z);
	Quaternion final = XMQuaternionRotationRollPitchYaw(def.x + m_factorX, def.y + m_factorY, def.z + m_factorZ);
	Vector3 fixedPos = XMQuaternionSlerp(def, final, 1);
	mGun.GetRotation() = Vector3(fixedPos.x, fixedPos.y + 2.66,fixedPos.z);
	
	Matrix objMtx;
	mGun.GetWorldMatrix(objMtx);
	Matrix camLock = objMtx * camLocalToWorld;

	// Clearing the Depth Buffer
	gd3dImmediateContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	FX::GetMyFX()->Render(mGun, gd3dImmediateContext, nullptr, &camLock);

	for (int i = 0; i < pBullets.size(); i++)
		pBullets[i]->Render();
}

Vector3 PlayerController::GetPos()
{
	return ptrCam.GetPos();
}

void PlayerController::initialise()
{
	mGun.Initialise(*GetMeshManager()->GetMesh("pistolmesh"));
	Vector3 mCameraStartPos = Vector3(15, -8.5, 12);
	MaterialExt& gMat = mGun.GetMesh().GetSubMesh(0).material;
	gMat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	gMat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("heavy.dds", true, gd3dDevice);
	gMat.texture = "heavy.dds";
	mGun.SetOverrideMat(&gMat);
	//MaterialExt& pistolMat = mGun.GetMesh().GetSubMesh(0).material;
	//pistolMat.flags &= ~MaterialExt::ZTEST;
	ptrCam.Initialise(mCameraStartPos, Vector3(0, 0, 1), FX::GetViewMatrix());
	ptrCam.LockMovementAxis(FPSCamera::UNLOCK, -9.5f, FPSCamera::UNLOCK);
}

void PlayerController::handleInputs(float dTime)
{
	bool reload(false), sprint(false), shoot(false);
	float forwardBack = 0, leftRight = 0;
	if (ptrCam.ptrLevel == nullptr)
	{
		ptrCam.ptrLevel = ptrLevel;
	}

	m_pressed = false;

	if (GetMouseAndKeys()->IsPressed(VK_W) && !GetMouseAndKeys()->IsPressed(VK_S)){
		forwardBack = 2;
		m_inputType = 0;
		m_pressed = true;
	}
	else if (GetMouseAndKeys()->IsPressed(VK_S) && !GetMouseAndKeys()->IsPressed(VK_W)) {
		forwardBack = -2;
		m_inputType = 0;
		m_pressed = true;
	}

	if (GetMouseAndKeys()->IsPressed(VK_A) && !GetMouseAndKeys()->IsPressed(VK_D)) {
		leftRight = 2;
		m_inputType = 0;
		m_pressed = true;
	}
	else if (GetMouseAndKeys()->IsPressed(VK_D) && !GetMouseAndKeys()->IsPressed(VK_A)) {
		leftRight = -2;
		m_inputType = 0;
		m_pressed = true;
	}
	
	sprint = GetMouseAndKeys()->IsPressed(VK_LSHIFT);
	interactDown = GetMouseAndKeys()->IsPressed(VK_E);
	flashlightDown = GetMouseAndKeys()->IsPressed(VK_F);
	reload = GetMouseAndKeys()->IsPressed(VK_R);

	shoot = GetMouseAndKeys()->GetMouseButton(MouseAndKeys::ButtonT::LBUTTON);
	ads = GetMouseAndKeys()->GetMouseButton(MouseAndKeys::ButtonT::RBUTTON);
	DirectX::SimpleMath::Vector2 m = GetMouseAndKeys()->GetMouseMoveAndCentre();
	if (m.x != 0 || m.y != 0)
	{
		m.x *= 0.05;
		m.y *= 0.05;
		m_inputType = 0;
		ptrCam.Rotate(dTime, m.x, m.y, 0);
	}
	if (m_inputType == 0) {
		m_factorX = m.y / m_mouseSwayFix;
		m_factorY = m.x / m_mouseSwayFix;
		m_factorZ = 0 * m_swayAmount;
	}

	ptrCam.Move(dTime, forwardBack, leftRight, sprint);

	mPad.Update();
	if (mPad.IsConnected(0)) //if not using gamepad just do kb+m
	{
		forwardBack = mPad.GetState(0).leftStickY * 2;
		leftRight = mPad.GetState(0).leftStickX * 2;
		sprint = mPad.IsPressed(0, XINPUT_GAMEPAD_LEFT_THUMB);
		reload = mPad.IsPressed(0, XINPUT_GAMEPAD_X);
		interactDown = mPad.IsPressed(0, XINPUT_GAMEPAD_B);
		flashlightDown = mPad.IsPressed(0, XINPUT_GAMEPAD_Y);
		if (mPad.GetState(0).rightTrigger > 0.25)
			shoot = true;
		if (mPad.GetState(0).leftTrigger > 0.25)
			ads = true;


		ptrCam.Move(dTime, forwardBack, -leftRight, sprint);
		//altered camera class to allow for "variable movement" instead of boolean movement directions
		
		float x = mPad.GetState(0).rightStickX;
		float y = mPad.GetState(0).rightStickY;

		if (x > 0 || x < 0)
			m_mouseIntputType = 1;
		if (x > 0 || x < 0)
			m_mouseIntputType = 1;

		x *= sense;
		y *= sense;
		if (x != 0 || y != 0)
			ptrCam.Rotate(dTime, x, -y, 0);

		m_MouseLastInput = false;
	}
	else {
		m_MouseLastInput = true;
	}

	//Sets the bool to see if playersmoved 
	if (forwardBack == 0 && leftRight == 0)
		playerMoved = false;
	else
		playerMoved = true;
	/////

	//Sound for when sprint is true and your moving.
	if (sprint == true && GetIAudioMgr()->GetSfxMgr()->IsPlaying(sfxHdlrun) == false && playerMoved == true)
	{
		
		GetIAudioMgr()->GetSfxMgr()->Play("RunMetal", false, false, &sfxHdlrun, sfxvolume);
	}
	else
	{
		if (sprint == false && GetIAudioMgr()->GetSfxMgr()->IsPlaying(sfxHdlrun) == true && playerMoved == false)
		{
			GetIAudioMgr()->GetSfxMgr()->Stop(sfxHdlrun);
		}
	}
	//////////
	

	
	//Sound when player is walking
	if (playerMoved == true && GetIAudioMgr()->GetSfxMgr()->IsPlaying(sfxHdlwalk) == false)
	{
		GetIAudioMgr()->GetSfxMgr()->Play("walkMetal", false, false, &sfxHdlwalk, sfxvolume);

	}
	else
	{
		if (playerMoved == false && GetIAudioMgr()->GetSfxMgr()->IsPlaying(sfxHdlwalk) == true)
		{
			GetIAudioMgr()->GetSfxMgr()->Stop(sfxHdlwalk);
		}
	}
	///////////////

	Vector3 camPosDir = ptrCam.GetPos();  //get which direction we are looking at
	Vector3 pLookDir = (ptrCam.dir);    //mouse direction - gun postion to get direction which bullet will go
	pLookDir.Normalize();

	coordX = ptrCam.GetPos().x / 10;
	coordZ = ptrCam.GetPos().y / 10; //get x,z to see what tile player is currently on



	if (flashlightDown)
	{
		if (!flashlightHeldDown)
		{
			flashlightOn = !flashlightOn;
		}
		flashlightHeldDown = true;
	}
	else
		flashlightHeldDown = false;


	flashlight();


	if (reload)
	{
		if (!reloadHeldDown) {
			ammoReload();
			empty = false;
		}
		reloadHeldDown = true;
	}
	else
		reloadHeldDown = false;

	if (shoot)
	{
		/*if (!rTriggerHeldDown) {
		Shoot(dTime, camPosDir, pLookDir);
		}
		rTriggerHeldDown = true;*/
		triggerReleasedSinceLastShot = false;
		Shoot(dTime, camPosDir, pLookDir);
	}
	else {
		//rTriggerHeldDown = false;
		triggerReleasedSinceLastShot = true;
		shotsRemainingInBurst = burstCount;
	}

	if (ads) {
		cGunPos = standardPos;
		mGun.GetPosition() = cGunPos;
	}
	else {
		cGunPos = adsPos;
		mGun.GetPosition() = cGunPos;
	}

	for (int i = 0; i < pBullets.size(); i++)
	{
		// Deleting all the time. Muppit.
		if (pBullets[i]->age > 10 || pBullets[i]->bCollision == false || pBullets[i]->pos.y >= -5.5 || pBullets[i]->pos.y <= -9.5)
		{
			delete(pBullets[i]);
			pBullets.erase(pBullets.begin() + i);
			i--;
		}
		else {
			pBullets[i]->Update(dTime);
		}
		//ptrPlayer->Update(dTime, forward, left, right, back, sprint);
	}

	if (m_inputType == 1) {
		m_factorX = mPad.GetState(0).rightStickY * m_swayAmount;
		m_factorY = mPad.GetState(0).rightStickX * m_swayAmount;
		m_factorZ = 0 * m_swayAmount;
	}
	
	if (m_factorX > m_maxSwayAmount)
		m_factorX = m_maxSwayAmount;
	if (m_factorX < -m_maxSwayAmount)
		m_factorX = -m_maxSwayAmount;
	if (m_factorY > m_maxSwayAmount)
		m_factorY = m_maxSwayAmount;
	if (m_factorY < -m_maxSwayAmount)
		m_factorY = -m_maxSwayAmount;
	if (m_factorZ > m_maxSwayAmount)
		m_factorZ = m_maxSwayAmount;
	if (m_factorZ < -m_maxSwayAmount)
		m_factorZ = -m_maxSwayAmount;
}
void PlayerController::flashlight() {
	if (flashlightOn)
	{

		FX::SetupSpotLight(1, true, ptrCam.GetPos(), ptrCam.GetRot(), Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 0, 0), 1000, 0.01, D2R(1), D2R(20));


	}
	else if (!flashlightOn)
	{
		FX::SetupSpotLight(1, false, ptrCam.GetPos(), ptrCam.GetRot(), Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 0, 0), 1000, 0.01, D2R(1), D2R(20));
	}
}
void PlayerController::takeDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		alive = false;
		GetIAudioMgr()->GetSfxMgr()->Play("player_death", false, false, nullptr, sfxvolume);
	}
	else
	{
		GetIAudioMgr()->GetSfxMgr()->Play("scream", false, false, nullptr, sfxvolume);
	}
}

void PlayerController::ammoReload() {
	if (mag > 0 && ammo != 30) {
		ammo = maxAmmo;
		mag--;
		GetIAudioMgr()->GetSfxMgr()->Play("Reload", false, false, nullptr, sfxvolume);
	}
}

void PlayerController::fireBullet(Vector3 bulletDirection, Vector3 position, float speed) {
	if (ammo > 0)
	{
		Bullet *Pbullet = new Bullet(position, bulletDirection, speed, pBullets.size(), ptrLevel);
		pBullets.push_back(Pbullet);
		ammo--;
		GetIAudioMgr()->GetSfxMgr()->Play("Gunshot", false, false, nullptr, sfxvolume);

		/*
		songName – no path, no extension, just the filename of the song
		loop – true or false, if true it won’t ever stop
		startPaused – true or false, if true it’s ready to go and just waiting for unpause
		musicHdl – unsigned int handle to the track, essential to stop or alter the playing track
		volume – float between 0 and 1, where 1 is full volume
		*/
	}
}

void PlayerController::muzzleFlashActivate(float dTime) {
	if (!muzzleFlash) {
		muzzleFlash = true;
		Update(dTime);
		while (currentFlashTime <= flashTime) {

			currentFlashTime += 1 * dTime;
		}
		muzzleFlashDeactivate();
	}

}

void PlayerController::muzzleFlashDeactivate() {
	if (muzzleFlash) {
		muzzleFlash = false;
		currentFlashTime = 0;
	}
}

void PlayerController::Shoot(float dTime, Vector3 camPosDir, Vector3 pLookDir) {
	if (empty) {
		if(GetIAudioMgr()->GetSfxMgr()->IsPlaying(sfxHdlgun) != true)
			GetIAudioMgr()->GetSfxMgr()->Play("handgundryfire", false, false, &sfxHdlgun, sfxvolume);
		return;
	}
	else
		if (tTime > nextShotTime) {
			if (fireMode == BURST) {
				if (shotsRemainingInBurst == 0) {
					return;
				}
				shotsRemainingInBurst--;
			}
			else if (fireMode == SINGLE) {
				if (!triggerReleasedSinceLastShot) {
					return;
				}
			}

			nextShotTime = tTime + msBetweenShots / 1000;
			fireBullet(pLookDir, camPosDir, muzzleVelocity);
			muzzleFlashActivate(dTime);
			if (!empty) {
				if (ammo == 0) {
					empty = true;
				}
			}
			/*mGun.GetRotation() -= Vector3(-1,0, -1) * m_kickMinMax.x;
			m_recoilAngle += m_recoilAngleMinMax.x;
			m_recoilAngle = clamp(m_recoilAngle, 0, 30);*/
		}
}

void PlayerController::RecoilHandle(float dTime) {
	/*mGun.GetRotation().x = lerp(cGunPos.x, 0, m_recoilRotSmoothDampVel);
	m_recoilAngle = lerp(m_recoilAngle, 0, m_recoilMoveSettleTime);
	mGun.GetRotation() = Vector3(0, 0, 1) * m_recoilAngle;*/
	//mGun.GetRotation().y = 267;
}

float PlayerController::lerp(float a, float b, float f) {
	return a + f * (b - a);
}

bool PlayerController::GetMouseLastInput() { return m_MouseLastInput; }
float PlayerController::GetControllerX() {
	return mPad.GetState(0).rightStickX;
	//DirectX::SimpleMath::Vector2 m = GetMouseAndKeys()->GetMouseMoveAndCentre();
	//return m.x;
}
float PlayerController::GetControllerY() {
	return mPad.GetState(0).rightStickY;
	//return m.x;
}
