#ifndef NodeH
#define NodeH

#include <math.h>

const int WORLD_SIZE = 100;
const int NODE_SIZE = 1;

struct Node {
public:
	int xPos, zPos;
	int id;
	Node *parent;
	float G; // dist start node and final node
	float H; // est. dist to goal

	Node() : parent(0) {};
	Node(int x, int z, Node *_parent = 0) : xPos(x), zPos(z),
		parent(_parent), id(z * WORLD_SIZE + x), G(0), H(0) {};

	float getF() { return G + H; }
	float mHDistance(int xP, int yP, Node *nodeEnd) {// dist from c node to t pos 'Known as Manhattan Distance'
		float x = (float)(fabs((float)(xP - nodeEnd->xPos)));
		float z = (float)(fabs((float)(yP - nodeEnd->zPos)));

		return x + z;
	}
};


#endif