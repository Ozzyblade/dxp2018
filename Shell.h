#ifndef ShellH
#define ShellH
#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "FPSCamera.h" // acts as the player
#include "SimpleMath.h"
#include <algorithm>
#include "bullet.h"
#include "FX.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Shell
{
public:
	Shell(Vector3 pos);
	~Shell() {
		Release();
	};

	void Initialise();
	void Update(float dTime);
	void Render(float dTime);
	void Release();

	Model mShell;
private:
	std::vector<Model*> mOpaques;


	Vector3 shell_position;
	float shell_life = 2.0;

	Vector3 mVel;
	Vector3 mGrav;
	Vector3 mAccel;
	float mCOR;

};


#endif // !ShellH

