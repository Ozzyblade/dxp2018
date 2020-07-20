#include "Shell.h"

Shell::Shell(Vector3 pos) {
	this->shell_position = pos;
	mVel = Vector3(0.25f, 0.9f, 0) * 3.5f;
	mGrav = Vector3(0, -9.81f, 0) * 0.5f;
	mAccel = Vector3(0, 0, 0);
	mCOR = 0.8f;
	this->Initialise();
}

void Shell::Initialise() {
	mShell.Initialise(*GetMeshManager()->GetMesh("bullet"));
	mShell.GetPosition() = shell_position;
	mShell.GetScale() = Vector3(0.0025, 0.0025, 0.0025);
	mShell.GetRotation() = Vector3(0, 0, 0);
	MaterialExt& shellMat = mShell.GetMesh().GetSubMesh(0).material;
	shellMat.flags &= ~MaterialExt::LIT;

	mOpaques.push_back(&mShell);
}

void Shell::Update(float dTime) {
	this->mVel += mGrav * dTime;
	this->shell_life -= 1 * dTime;
}

void Shell::Render(float dTIme) {
	FX::GetMyFX()->Render(this->mShell, gd3dImmediateContext);
}

void Shell::Release() {

}