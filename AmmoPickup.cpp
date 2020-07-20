#include "AmmoPickup.h"

AmmoPickup::AmmoPickup(PlayerController *player) {
	m_Player = player;
	SetPlayer(m_Player);
	m_MagsToAdd = 1;
	m_Pad.Initialise();
	Initialise();
}

void AmmoPickup::Initialise() {
	mModel.Initialise(*GetMeshManager()->GetMesh("icon_Ammo"));
	mModel.GetPosition() = GetPos();
	mModel.GetScale() = Vector3(0.03, 0.03, 0.03);
	MaterialExt& mAmmoMat = mModel.GetMesh().GetSubMesh(0).material;
	mAmmoMat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mAmmoMat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("drone02.dds", true, gd3dDevice);
	mAmmoMat.texture = "drone02.dds";
	mModel.SetOverrideMat(&mAmmoMat);
}

void AmmoPickup::Update(float dTime) {
	mModel.GetRotation() = Vector3(0, GetClock() / 2, 0);
	m_Pad.Update();

	if (CanInteract() && GetActive()) {
		if (GetMouseAndKeys()->IsPressed(VK_E)) {
			Interact();
		}

		if (m_Pad.IsConnected(0)) {
			if (m_Pad.IsPressed(0, XINPUT_GAMEPAD_B))
				Interact();
		}
	}

	if (GetWithinLightRange()) {
		FX::SetupPointLight(GetIndex(), GetActive(), GetPos(), Vector3(0, 0, 1), Vector3(0, 0, 1), Vector3(0, 0, 0), GetTriggerSize(), 0.05f);
	}
}

void AmmoPickup::Release() {

}

void AmmoPickup::Render() {
	if (GetActive()) {
		FX::GetMyFX()->Render(mModel, gd3dImmediateContext);
	}
}

void AmmoPickup::Interact() {
	SetActive(false);
	if (GetWithinLightRange() && GetActive()) 
		FX::SetupPointLight(GetIndex(), GetActive(), GetPos(), Vector3(0, 0, 1), Vector3(0, 0, 1), Vector3(0, 0, 0), GetTriggerSize(), 0.05f);
	if ((m_Player->mag + m_MagsToAdd) >= m_Player->magMax)
		m_Player->mag = m_Player->magMax;
	else
		m_Player->mag += m_MagsToAdd;
}