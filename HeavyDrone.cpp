#include "HeavyDrone.h"
#include "GeometryBuilder.h"
#include "FX.h"
#include "Game.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


HeavyDrone::HeavyDrone(PlayerController *player) {
	target = player;
}

void HeavyDrone::Initialise(){
	SetStartPos(GetPos());
	target_pos = target->GetPos();
	ptrLevel = target->ptrLevel;
	SetScale(GetScale() * GetScaleFactor());

	mDroneBody.Initialise(*GetMeshManager()->GetMesh("drone04_body"));
	mDroneBody.GetPosition()	= GetPos();
	mDroneBody.GetScale()		= GetScale();
	mDroneBody.GetRotation()	= GetDir();
	MaterialExt& dronem = mDroneBody.GetMesh().GetSubMesh(0).material;
	dronem.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	dronem.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("drone04_body.dds", true, gd3dDevice);
	dronem.texture = "drone04_body.dds";
	mDroneBody.SetOverrideMat(&dronem);

	mDroneGunL.Initialise(*GetMeshManager()->GetMesh("drone04_gun_L"));
	mDroneGunL.GetPosition()	= GetPos() + droneGunLOffset;
	mDroneGunL.GetScale()		= GetScale();
	mDroneGunL.GetRotation()	= GetDir();
	MaterialExt& droneGMat = mDroneGunL.GetMesh().GetSubMesh(0).material;
	droneGMat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	droneGMat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("drone04_gun.dds", true, gd3dDevice);
	droneGMat.texture = "drone04_gun.dds";
	mDroneGunL.SetOverrideMat(&droneGMat);
	
	mDroneGunR.Initialise(*GetMeshManager()->GetMesh("drone04_gun_R"));
	mDroneGunR.GetPosition()	= GetPos() - droneGunLOffset;
	mDroneGunR.GetScale()		= GetScale();
	mDroneGunR.GetRotation()	= GetDir();
	mDroneGunR.SetOverrideMat(&droneGMat);
	
	
	mOpaques.push_back(&mDroneBody);
	mOpaques.push_back(&mDroneGunL);
	mOpaques.push_back(&mDroneGunR);

}



void HeavyDrone::Update(float dTime) {
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

void HeavyDrone::Attack(float dTime, Vector3 pos) {
	m_shotCount -= 2 * dTime;
	if (m_shotCount <= 0 && Vector3::Distance(GetPos(), target_pos) < 10) {
		Vector3 shotDir = target_pos - GetPos();
		Bullet *bullet = new Bullet(GetPos(), shotDir, 1, bullets.size(), ptrLevel);
		bullet->Update(dTime);
		bullets.push_back(bullet);
		m_shotCount = 2;
	}
}

void HeavyDrone::UpdateShots(float dTime) {
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

void HeavyDrone::UpdateEnemyMesh(float dTime) {
	mDroneBody.GetRotation().y = m_rotAxis + 3.14;

	Vector3 lookDir = Vector3(target_pos - GetPos());

	mDroneGunL.GetRotation().y = mDroneBody.GetRotation().y;
	mDroneGunR.GetRotation().y = mDroneBody.GetRotation().y;
	mDroneGunR.GetRotation().x = lookDir.y + 1.14;
	mDroneGunL.GetRotation().x = lookDir.y + 1.14;

	mDroneGunL.GetPosition() = mDroneBody.GetPosition() + droneGunLOffset;
	mDroneGunR.GetPosition() = mDroneBody.GetPosition() - droneGunLOffset;
}

void HeavyDrone::Die() {
	SetActive(false);
	target->score += 25;
}

void HeavyDrone::Render() {
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->Render();
	
	FX::GetMyFX()->Render(mDroneBody, gd3dImmediateContext);
	FX::GetMyFX()->Render(mDroneGunL, gd3dImmediateContext);
	FX::GetMyFX()->Render(mDroneGunR, gd3dImmediateContext);
}

void HeavyDrone::Release() {
	for (int i = 0; i < bullets.size(); i++)
		delete bullets[i];
}
