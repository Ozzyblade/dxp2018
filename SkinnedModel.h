#ifndef SkinnedModelH
#define SkinnedModelH

#include "SkinnedData.h"
#include "MeshGeometry.h"
#include "Vertex.h"

class SkinnedModel {
public:
	SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath);
	~SkinnedModel();

	UINT SubsetCount;

	vector<Material> Mat;
	vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	vector<ID3D11ShaderResourceView*> NormalMapSRV;

	// Keep CPU copies of the mesh data to read from.  
	vector<PosNormalTexTanSkinned> Vertices;
	vector<USHORT> Indices;
	vector<Subset> Subsets;

	MeshGeometry ModelMesh;
	SkinnedData SkinnedData;
};

struct SkinnedModelInstance
{
	SkinnedModel* Model;
	float TimePos;
	std::string ClipName;
	XMFLOAT4X4 World;
	std::vector<XMFLOAT4X4> FinalTransforms;

	void Update(float dt);
};

#endif // !SkinnedModelH

