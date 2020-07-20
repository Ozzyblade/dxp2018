#ifndef EntityHandlerH
#define EntityHandlerH

#include "Interactable.h"
#include "AmmoPickup.h"
#include "HealthPickup.h"
#include "Objective.h"
#include "Enemy.h"
#include "LightDrone.h"
#include "HeavyDrone.h"
#include <vector>

class EntityHandler {
public:
	EntityHandler();
	~EntityHandler() {
		Release();
	}

	void Initialise(PlayerController *tar);
	void Update(float dTime);
	vector<Objective*> getObjectives() { return m_objectives; }
	void Render();
	void Release();
	int	 GetEntityUIState();
	int GetObjectiveCount();
private:
	vector<Interactable*>	m_interactableList; //for ammo and health
	vector<Enemy*>			m_enemyList;
	vector<Objective*>      m_objectives;

	PlayerController *player;

	vector<int> m_lightIndxClsd;
	vector<int> m_lightIndxOpn;

	int m_uiState = 0;
	float m_lightRange = 20;

	void SetAllAttributes();
	int  GetOpenIndex();
	void CheckIndexRange();
	void PlayerEnemyBulletCollision(float dTime);

	int objectiveSize = 3;
};

#endif