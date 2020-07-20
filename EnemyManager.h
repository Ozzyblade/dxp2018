#ifndef EnemyManagerH
#define EnemyManagerH

#include "HeavyDrone.h";
class EnemyManager
{
public:
	EnemyManager(FPSCamera target);
	~EnemyManager() {
		Release();
	};

	void Initalise();
	void Update(float dTime, FPSCamera target);
	void Render(float dTime);
	void Release();

	vector<HeavyDrone*>enemies;
	vector<Vector3> startPos;
	FPSCamera target;
private:
	float drone_spd = 0.5;
	Vector3 startDir = Vector3(0, 0, 1);
};

#endif
