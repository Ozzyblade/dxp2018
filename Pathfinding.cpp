#include "Pathfinding.h"

Pathfinding::Pathfinding() {
	initializedStartGoal = false;
	foundGoal = false;
}

void Pathfinding::FindPath(Vector3 currentPos, Vector3 targetPos) {
	if (!initializedStartGoal) {
		for (int i = 0; i < openList.size(); i++) {
			delete openList[i];
		}
		openList.clear();

		for (int i = 0; i < visitedList.size(); i++) {
			delete visitedList[i];
		}
		visitedList.clear();

		for (int i = 0; i < pathToGoal.size(); i++) {
			delete pathToGoal[i];
		}
		pathToGoal.clear();

		// initialse start
		Node start;
		start.xPos = currentPos.x;
		start.zPos = currentPos.z;

		// initialise goal
		Node goal;
		goal.xPos = targetPos.x;
		goal.zPos = targetPos.z;

		//------PROPERLY-IMPLEMENT-----
		//start.xPos = gameWorld->getNodeX(currentPos.x);
		//start.zPos = gameWorld->getNodeY(currentPos.z);

		//end.xPos = gameWorld->getNodeX(targetPos.x);
		//end.zPos = gameWorld->getNodeY(targetPos.z);

		foundGoal = false;
		SetStartGoal(start, goal); 
		initializedStartGoal = true;
	}

	if (initializedStartGoal) {
		ContinuePath();
	}
}

void Pathfinding::SetStartGoal(Node start, Node goal) {
	startNode	= new Node(start.xPos, start.zPos, NULL);
	goalNode	= new Node(goal.xPos, goal.zPos, &goal);

	startNode->G = 0;
	startNode->H = startNode->mHDistance(start.xPos, start.zPos, goalNode);
	startNode->parent = 0;

	openList.push_back(startNode);
}

Node* Pathfinding::GetNextNode() {
	float bestF		= 99999.0f;
	int nodeIndex	= -1;
	Node* nextNode	= NULL;

	for (int i = 0; i < openList.size(); i++) {
		if (openList[i]->getF() < bestF) {
			bestF = openList[i]->getF();
			nodeIndex = i;
		}
	}

	if (nodeIndex >= 0) {
		nextNode = openList[nodeIndex];
		visitedList.push_back(nextNode);
		openList.erase(openList.begin() + nodeIndex);
	}

	return nextNode;
}

void Pathfinding::PathOpened(int x, int z, float newCost, Node *parent) {
	//if (CELL_BLOCKED) {
	//	return;
	//}

	int id = z * WORLD_SIZE + x;

	for (int i = 0; i < visitedList.size(); i++) {
		if (id == visitedList[i]->id) {
			return;
		}
	}

	Node* newChild = new Node(x, z, parent);
	newChild->G = newCost;
	newChild->H = parent->mHDistance(x, z, goalNode);

	for (int i = 0; i < openList.size(); i++) {
		if (id == openList[i]->id) {
			float newF = newChild->G + newCost + openList[i]->H;

			if (openList[i]->getF() > newF) {
				openList[i]->G = newChild->G + newCost;
				openList[i]->parent = newChild;
			}
			else {
				delete newChild;
				return;
			}
		}
	}

	openList.push_back(newChild);
}

void Pathfinding::ContinuePath() {

	if (openList.empty())
		return;

	Node* currentNode = GetNextNode();

	if (currentNode->id == goalNode->id) {
		goalNode->parent = currentNode->parent; 

		Node* getPath;

		for (getPath = goalNode; getPath != NULL; getPath = getPath->parent) {
			pathToGoal.push_back(new Vector3(getPath->xPos * 10, -8.5f, getPath->zPos * 10));
			//pathToGoal.push_back(new Vector3(getPath->xPos * CELL_SIZE, -8.5f, getPath->zPos * CELL_SIZE));
		}

		foundGoal = true;
		return;
	}
	else {
		// rightNode
		PathOpened(currentNode->xPos + 1, currentNode->zPos, currentNode->G + 1, currentNode->parent);
		// leftNode
		PathOpened(currentNode->xPos - 1, currentNode->zPos, currentNode->G + 1, currentNode->parent);
		// upNode
		PathOpened(currentNode->xPos, currentNode->zPos + 1, currentNode->G + 1, currentNode->parent);
		// bottomNode
		PathOpened(currentNode->xPos, currentNode->zPos - 1, currentNode->G + 1, currentNode->parent);
		// left_up Diag
		PathOpened(currentNode->xPos - 1, currentNode->zPos + 1, currentNode->G + 1.414f, currentNode->parent);
		// right_up Diag
		PathOpened(currentNode->xPos + 1, currentNode->zPos + 1, currentNode->G + 1.414f, currentNode->parent);
		// left_down Diag
		PathOpened(currentNode->xPos - 1, currentNode->zPos -1, currentNode->G + 1.414f, currentNode->parent);
		//right_down Diag
		PathOpened(currentNode->xPos + 1, currentNode->zPos -1, currentNode->G + 1.414f, currentNode->parent);

		for (int i = 0; i < openList.size(); i++) {

			if (currentNode->id == openList[i]->id) 
				openList.erase(openList.begin() + 1);
		}
	}
}

Vector3 Pathfinding::NextPathPos(Vector3 pos, float radius) {
	int index = 1;

	Vector3 nextPos;
	nextPos.x = pathToGoal[pathToGoal.size() - index]->x;
	nextPos.y = -8.5f;
	nextPos.z = pathToGoal[pathToGoal.size() - index]->z;

	Vector3 distance = nextPos - pos; // Ai Pos

	if (index < pathToGoal.size()) {
		if (distance.Length() < radius) { // Ai Radius
			pathToGoal.erase(pathToGoal.end() - index);
		}
	}

	return nextPos;
}

Vector3 Pathfinding::GetRandomizeGoal() {
	int randomX = (int)(rand() % WORLD_SIZE * NODE_SIZE);
	int randomZ = (int)(rand() % WORLD_SIZE * NODE_SIZE);

	return Vector3(randomX, -8.5, randomZ);
}

double Pathfinding::rad2Deg(int x) {
	double degrees;
	double radians;

	degrees = radians * 180 / 3.14159265;
	return degrees;
}

void Pathfinding::Release() {

}