#ifndef BulletH
#define BulletH
#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "FPSCamera.h" // acts as the player
#include "SimpleMath.h"
#include <algorithm>
#include "Level.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Bullet {
public:
	Vector3 startPos;
	Vector3 pos;
	Vector3 dir;
	float min_dist;
	int counter;
	float speed;
	float age;

	bool bCollision;
	Level* lCollision;	// This create's it's own version of the level. We might want a pointer to the original level instead.


	Bullet(Vector3 pos, Vector3 dir, float speed, int counter, Level* lptr);
	void Render();
	void Initalise();
	void Update(float dTime);
	Model mBulletTest;
	bool isActive() { return active; }
	void collision(float x, float z);

private:
	std::vector<Model*> mOpaques;

	bool active = false;
};

#endif // !BulletH
