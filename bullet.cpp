#include "bullet.h"
#include "GeometryBuilder.h"
#include "FX.h"
#include "Game.h"

Bullet::Bullet(Vector3 pos, Vector3 dir, float speed, int counter, Level* lptr) {
	this->startPos = pos;
	this->pos = startPos;
	this->counter = 0; 
	this->speed = speed;
	this->age = 0;
	this->dir = dir;
	this->bCollision;
	this->lCollision = lptr;
	Initalise();
}

void Bullet::Initalise() {
	
	mBulletTest.Initialise(*GetMeshManager()->GetMesh("bullet"));
	mBulletTest.GetPosition() = startPos;
	mBulletTest.GetScale() = Vector3(0.08, 0.08, 0.08);
	mBulletTest.GetRotation() = Vector3(0, 0, 0);
	MaterialExt& bulletMat = mBulletTest.GetMesh().GetSubMesh(0).material;
	//coinMat.flags | MaterialExt::LIT;
	bulletMat.flags &= ~MaterialExt::LIT;
	//defMat.flags &= ~MaterialExt::ZTEST;
	mOpaques.push_back(&mBulletTest);
	//Render();
}

void Bullet::Render() {
	FX::GetMyFX()->Render(this->mBulletTest, gd3dImmediateContext);
}

void Bullet::Update(float dTime) {
	this->age+= 1*dTime;
	this->pos.x += dir.x*speed * dTime;
	this->pos.y += dir.y*speed * dTime;
	this->pos.z += dir.z*speed * dTime;
	this->mBulletTest.GetPosition() = pos;
	this->collision(pos.x, pos.z);
}
//
void Bullet::collision(float x, float z) {
	
	if (lCollision->LevelCollision(x, z) == true) {  //if true you can carry on moving
		this->bCollision = true;
	}
	else
		this->bCollision = false;   //if false then your osmewhere you shouldnt be
}