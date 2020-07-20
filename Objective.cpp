#include "Objective.h"

Objective::Objective(PlayerController *player, wstring msg) {
	m_Player = player;
	message = msg;
	SetPlayer(m_Player);
	m_ObjectiveClear = false;
	m_Pad.Initialise();
}

void Objective::Initialise() {
	mModel.Initialise(*GetMeshManager()->GetMesh("icon_Spanner"));
	mModel.GetPosition() = GetPos();
	mModel.GetScale() = Vector3(0.03, 0.03, 0.03);
	MaterialExt& mHealthMat = mModel.GetMesh().GetSubMesh(0).material;
	mHealthMat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mHealthMat.pTextureRV = FX::GetMyFX()->mCache.LoadTexture("drone02.dds", true, gd3dDevice);
	mHealthMat.texture = "drone02.dds";
	mModel.SetOverrideMat(&mHealthMat);
}

void Objective::Update(float dTime) {
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
		FX::SetupPointLight(GetIndex(), GetActive(), GetPos(), Vector3(1, 1, 0), Vector3(1, 1, 0), Vector3(0, 0, 0), GetTriggerSize(), 0.05f);
	}
}

void Objective::Release() {
}

void Objective::Render() {
	if (GetActive()) {
		FX::GetMyFX()->Render(mModel, gd3dImmediateContext);
	}
}

void Objective::Interact() {
	SetActive(false); 
	
}

bool Objective::GetObjectiveClear() { return m_ObjectiveClear; }