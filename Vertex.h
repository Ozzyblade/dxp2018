//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************

#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"
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

namespace Vertex
{
	// Basic 32-byte vertex structure.
	struct Basic32
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct PosNormalTexTan
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};

	struct PosNormalTexTanSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		XMFLOAT3 Weights;
		BYTE BoneIndices[4];
	};
}

class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC Pos[1];
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTan[4];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTanSkinned[6];
};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* Basic32;
	static ID3D11InputLayout* PosNormalTexTan;
	static ID3D11InputLayout* PosNormalTexTanSkinned;
};

#endif // VERTEX_H