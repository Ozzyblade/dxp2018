#ifndef AnimationHelperH
#define AnimationHelperH

#include <vector>
#include "Mesh.h"
#include "Model.h"
#include "FPSCamera.h" // acts as the player
#include "SimpleMath.h"
#include <algorithm>
#include "bullet.h"
#include "BoneAnimation.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Keyframe {
	Keyframe();
	~Keyframe();

	float TimePos;
	XMFloat3 Translation;
	XMFloat3 Scale;
	XMFloat4 RotationQuat;
};

struct BoneAnimation {
	float GetStartTime() const;
	float GetEndTime() const;
	void Interpolate(float t, XMFLOAT4X4& M) const;
	vector<Keyframe> Keyframes;
};

#endif // !AnimationHelperH
