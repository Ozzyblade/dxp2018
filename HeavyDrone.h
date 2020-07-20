#ifndef HeavyDroneH
#define HeavyDroneH

#include "Enemy.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class HeavyDrone : public Enemy{
public:
	HeavyDrone(PlayerController *player);
	~HeavyDrone() {
		Release();
	};

	void Initialise();
	void Update(float dTime);
	void Render();
	void Release();
	bool active;
	virtual vector<Bullet*> getBullets() { return bullets; }
	vector<Bullet*>bullets;
	//Vector3 calculateRotation();
	Level* ptrLevel;
	void Attack(float dTime, Vector3 pos);
	void Die();
	void UpdateShots(float dTime);

private:
	PlayerController * target;
	std::vector<Model*> mOpaques;
	Model mDroneBody, mDroneGunL, mDroneGunR;

	Vector3 droneGunLOffset = Vector3(0.05, 0, 0);

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
#endif // !EnemyH

