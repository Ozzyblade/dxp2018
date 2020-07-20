#ifndef SkinnedDataH
#define SkinnedDataH

#include "D3DUtil.h"
#include <map>
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

struct Keyframe 
{
	Keyframe();
	~Keyframe();

	float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	float GetStartTime() const;
	float GetEndTime() const;

	void Interpolate(float t, XMFLOAT4X4& M) const;
	vector<Keyframe> Keyframes;
};

struct AnimationClip 
{
	float GetClipStartTime() const;
	float GetClipEndTime() const;

	void Interpolate(float t, vector<XMFLOAT4X4>& boneTransforms) const;
	vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:
	SkinnedData();
	~SkinnedData();

	UINT BoneCount() const;

	float GetClipStartTime(const string& clipName) const;
	float GetClipEndTime(const string& clipName) const;

	void Set(
		vector<int>& boneHierarchy,
		vector<XMFLOAT4X4>& boneOffsets,
		map<string, AnimationClip>& animations);

	void GetFinalTransforms(const string& clipName, float timePos, 
		vector<XMFLOAT4X4>& finalTransforms) const;

private:
	vector<int> mBoneHierarchy;
	vector<XMFLOAT4X4> mBoneOffsets;
	map<string, AnimationClip> mAnimations;
};
#endif // !SkinnedData.h