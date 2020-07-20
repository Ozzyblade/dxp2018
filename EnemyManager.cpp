// Handles all enemies in the game
#include "EnemyManager.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

Vector3 positions[2] = { Vector3(16,12,-8.50), Vector3(60,-9,25) };

EnemyManager::EnemyManager(FPSCamera target) {
	for (int i = 0; i < 2; i++) {
		//HeavyDrone *enemy_drone = new HeavyDrone(target, drone_spd, startDir, positions[0]);
		//enemies.push_back(enemy_drone);
	}
}

void EnemyManager::Initalise() {
	for (int i = 0; i < enemies.size(); i++) {
	//	enemies.at(i)->Initalise();
	}
}

void EnemyManager::Update(float dTime, FPSCamera target) {
	for (int i = 0; i < enemies.size(); i++) {
		//enemies.at(i)->Update(dTime, target);
	}
}

void EnemyManager::Render(float dTime) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i)->Render(dTime);
	}
}

void EnemyManager::Release() {
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i)->Release();
	}
}