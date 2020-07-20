#ifndef LightDroneH
#define LightDroneH

#include "Enemy.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class LightDrone : public Enemy
{
public:
	LightDrone(PlayerController *player);
	~LightDrone() {
		Release();
	};

	void Initialise();
	void Update(float dTime);
	void Render();
	void Release();
	void Attack(float dTime, Vector3 pos);
	void Die();
	void UpdateShots(float dTime);
	virtual vector<Bullet*> getBullets() { return bullets; }
	vector<Bullet*>bullets;
	Level* ptrLevel;
private:
	PlayerController * target;
	std::vector<Model*> mOpaques;
	Model mDroneBody, mDroneL, mDroneR, mDroneLG, mDroneRG;

	Vector3 dGunOffset = Vector3(-0.1, 0, 0);
	Vector3 dGunOffsetI = Vector3(-0.5, -0.2, 0);

	Vector3 dCGunOffset = dGunOffset;
	Vector3 dCGunOffsetI = dGunOffsetI;

	Vector3 currentPos;
	Vector3 target_pos;
	bool isShotLeft;
	bool attacking;
	void UpdateEnemyMesh(float dTime);
	
	float idleCount = 0;
	float dist;
	Vector3 lookDir;
	int patrolPoint = 1;
	float m_shotCount = 0;
	float m_shotSpd = 40;
	float m_rotAxis = 0;
};

#endif // !LightDroneH
