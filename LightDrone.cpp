#include "LightDrone.h"
LightDrone::LightDrone(PlayerController *player)
{
	target = player;
}

void LightDrone::Initialise() {
	SetStartPos(GetPos());
	target_pos = target->GetPos();

	ptrLevel = target->ptrLevel;

	SetScale(GetScale() * GetScaleFactor());

	mDroneBody.Initialise(*GetMeshManager()->GetMesh("drone02_body"));
	mDroneBody.GetPosition()	= GetPos();
	mDroneBody.GetScale()		= GetScale();
	mDroneBody.GetRotation()	= GetDir();
	MaterialExt& dBodyMat		= mDroneBody.GetMesh().GetSubMesh(0).material;
	dBodyMat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	dBodyMat.pTextureRV			= FX::GetMyFX()->mCache.LoadTexture("drone02.dds", true, gd3dDevice);
	dBodyMat.texture			= "drone02.dds";
	mDroneBody.SetOverrideMat(&dBodyMat);

	mDroneL.Initialise(*GetMeshManager()->GetMesh("drone02_left"));
	mDroneL.GetPosition()	= GetPos() + dCGunOffset;
	mDroneL.GetScale()		= GetScale();
	mDroneL.GetRotation()	= GetDir();
	mDroneL.SetOverrideMat(&dBodyMat);

	mDroneR.Initialise(*GetMeshManager()->GetMesh("drone02_right"));
	mDroneR.GetPosition()	= GetPos() - dCGunOffset;
	mDroneR.GetScale()		= GetScale();
	mDroneR.GetRotation()	= GetDir();
	mDroneR.SetOverrideMat(&dBodyMat);

	mDroneLG.Initialise(*GetMeshManager()->GetMesh("drone02_left_g"));
	mDroneLG.GetPosition()	= GetPos() + dCGunOffsetI;
	mDroneLG.GetScale()		= GetScale();
	mDroneLG.GetRotation()	= GetDir();
	mDroneLG.SetOverrideMat(&dBodyMat);

	mDroneRG.Initialise(*GetMeshManager()->GetMesh("drone02_right_g"));
	mDroneRG.GetPosition()	= GetPos() - dCGunOffsetI;
	mDroneRG.GetScale()		= GetScale();
	mDroneRG.GetRotation()	= GetDir();
	mDroneRG.SetOverrideMat(&dBodyMat);

	mOpaques.push_back(&mDroneBody);
	mOpaques.push_back(&mDroneL);
	mOpaques.push_back(&mDroneR);
	mOpaques.push_back(&mDroneLG);
	mOpaques.push_back(&mDroneRG);
}

void LightDrone::Update(float dTime) {
	if (GetActive()) {

		SetPos(mDroneBody.GetPosition());

		float distToPlayer = Vector3::Distance(mDroneBody.GetPosition(), target->GetPos());
		float distance = Vector3::Distance(GetStartPos(), mDroneBody.GetPosition());

		m_rotAxis = GetRotPos(target_pos);
		UpdateEnemyMesh(dTime);

		if (distance < GetTether()) {
			//SetSpeed(0.5f);
		}
		else {
			mDroneBody.GetPosition() += (GetStartPos() - mDroneBody.GetPosition()) * GetSpeed() * dTime;
		}

		if (distToPlayer >= GetAttackRange()) {
			target_pos = GetStartPos();
		}
		else {
			target_pos = target->GetPos();
			Attack(dTime, mDroneBody.GetPosition());
		}

		mDroneBody.GetPosition() += (target_pos - mDroneBody.GetPosition()) * GetSpeed() * dTime;

		CheckCollision();
	}
	UpdateShots(dTime);
}

void LightDrone::UpdateShots(float dTime) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->age > 10 || !bullets[i]->bCollision) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}
		else {
			bullets[i]->Update(dTime);
		}
	}
}

void LightDrone::Attack(float dTime, Vector3 pos) {
	m_shotCount -= 1 * dTime;
	if (m_shotCount <= 0 && Vector3::Distance(GetPos(), target_pos) < 10) {
		Vector3 shotDir = target_pos - GetPos();
		Bullet *bullet = new Bullet(GetPos(), shotDir, 1, bullets.size(), ptrLevel);
		bullet->Update(dTime);
		bullets.push_back(bullet);
		m_shotCount = 1;
	}
}

void LightDrone::Die() {
	SetActive(false);
	target->score += 15;
}

void LightDrone::UpdateEnemyMesh(float dTime) {
	mDroneBody.GetRotation().y = m_rotAxis + 3.14;

	mDroneL.GetRotation()	= mDroneBody.GetRotation();
	mDroneR.GetRotation()	= mDroneBody.GetRotation();
	mDroneLG.GetRotation()	= mDroneBody.GetRotation();
	mDroneRG.GetRotation()	= mDroneBody.GetRotation();
	mDroneL.GetPosition()	= mDroneBody.GetPosition() + dCGunOffset;
	mDroneR.GetPosition()	= mDroneBody.GetPosition() - dCGunOffset;
	mDroneLG.GetPosition()	= mDroneBody.GetPosition() + dCGunOffset;
	mDroneRG.GetPosition()	= mDroneBody.GetPosition() - dCGunOffset;
}

void LightDrone::Render() {
	for (int i = 0; i < bullets.size(); i++)
		bullets.at(i)->Render();
	if (GetActive()) {
		FX::GetMyFX()->Render(mDroneBody, gd3dImmediateContext);
		FX::GetMyFX()->Render(mDroneL, gd3dImmediateContext);
		FX::GetMyFX()->Render(mDroneR, gd3dImmediateContext);
		FX::GetMyFX()->Render(mDroneLG, gd3dImmediateContext);
		FX::GetMyFX()->Render(mDroneRG, gd3dImmediateContext);
	}
}

void LightDrone::Release() {
	for (int i = 0; i < bullets.size(); i++)
		delete bullets[i];
}