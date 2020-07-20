#ifndef PlayerH
#define PlayerH

#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "FPSCamera.h" // acts as the player

class Player {
public:
	Player();
	

	void Update(float dTime, float,float,float,float,bool);
	void Render(float dTime);
	void Fire();
	void Initalise();
	void Release();

	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Model mPlayer;

private:
	
};
#endif // !EnemyH