#ifndef PathfinderH
#define PathfinderH

#include <vector>
#include "d3d12.h"
#include "SimpleMath.h"
#include "Node.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding() {
		Release();
	};

	void Release();

	void FindPath(Vector3 currentPos, Vector3 targetPos);
	Vector3 NextPathPos(Vector3 pos, float radius);

	void ClearOpenList()	{ openList.clear(); }
	void ClearVisitedList() { visitedList.clear(); }
	void ClearPathToGoal()	{ pathToGoal.clear(); }
	Vector3 GetRandomizeGoal();

	bool initializedStartGoal;
	bool foundGoal;
	bool isAtGoal;

	// helpers
	double rad2Deg(int x);

private:
	void SetStartGoal(Node start, Node goal);
	void PathOpened(int x, int z, float newCost, Node *parent);
	Node *GetNextNode();
	void ContinuePath();	

	Node *startNode;
	Node *goalNode;
	vector<Node*> openList;
	vector<Node*> visitedList;
	vector<Vector3*> pathToGoal;
};
#endif // !PathfinderH
