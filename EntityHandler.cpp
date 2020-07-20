// Handles all spawnable entities 
#include "EntityHandler.h"

EntityHandler::EntityHandler() {
}

void EntityHandler::Initialise(PlayerController *tar) {
	player = tar;

	// Creation of enemies
	Enemy *m_Enemy1 = new HeavyDrone(player);
	Enemy *m_Enemy2 = new LightDrone(player);
	Enemy *m_Enemy3 = new HeavyDrone(player);
	Enemy *m_Enemy4 = new HeavyDrone(player);
	Enemy *m_Enemy5 = new LightDrone(player);
	Enemy *m_Enemy6 = new LightDrone(player);
	Enemy *m_Enemy7 = new LightDrone(player);
	Enemy *m_Enemy8 = new HeavyDrone(player);
	Enemy *m_Enemy9 = new HeavyDrone(player);
	Enemy *m_Enemy10 = new LightDrone(player);
	Enemy *m_Enemy11 = new HeavyDrone(player);


	m_enemyList.push_back(m_Enemy1);
	m_enemyList.push_back(m_Enemy2);
	m_enemyList.push_back(m_Enemy3);
	m_enemyList.push_back(m_Enemy4);
	m_enemyList.push_back(m_Enemy5);
	m_enemyList.push_back(m_Enemy6);
	m_enemyList.push_back(m_Enemy7);
	m_enemyList.push_back(m_Enemy8);
	m_enemyList.push_back(m_Enemy9);
	m_enemyList.push_back(m_Enemy10);
	m_enemyList.push_back(m_Enemy11);

	// Creation of interactable objects
	Objective *m_Interactable1 = new Objective(player, L"FIND THE TOOLS");
	m_objectives.push_back(m_Interactable1);
	Objective *m_Interactable2 = new Objective(player, L"FIX THE ENGINE");
	m_objectives.push_back(m_Interactable2);
	Objective *m_Interactable3 = new Objective(player, L"FIND THE POD");
	m_objectives.push_back(m_Interactable3);
	Interactable *m_Interactable4 = new AmmoPickup(player);
	Interactable *m_Interactable5 = new AmmoPickup(player);
	Interactable *m_Interactable6 = new AmmoPickup(player);
	Interactable *m_Interactable7 = new HealthPickup(player);
	Interactable *m_Interactable8 = new HealthPickup(player);
	Interactable *m_Interactable9 = new HealthPickup(player);

	m_interactableList.push_back(m_Interactable1);
	m_interactableList.push_back(m_Interactable2);
	m_interactableList.push_back(m_Interactable3);
	m_interactableList.push_back(m_Interactable4);
	m_interactableList.push_back(m_Interactable5);
	m_interactableList.push_back(m_Interactable6);
	m_interactableList.push_back(m_Interactable7);
	m_interactableList.push_back(m_Interactable8);
	m_interactableList.push_back(m_Interactable9);

	SetAllAttributes();

	for (int i = 0; i < m_enemyList.size(); i++) {
		if (m_enemyList.at(i)->GetActive()) {
			m_enemyList.at(i)->Initialise();
		}
	}

	for (int i = 0; i < m_interactableList.size(); i++) {
		if (m_interactableList.at(i)->GetActive()) {
			m_interactableList.at(i)->Initialise();
		}
	}
}

void EntityHandler::Update(float dTime) {
	for (int i = 0; i < m_interactableList.size(); i++) {
		if (m_interactableList.at(i)->GetActive()) {
			m_interactableList.at(i)->Update(dTime);
		}
	}	

	for (int i = 0; i < m_enemyList.size(); i++) {
		if (m_enemyList.at(i)->GetActive()) {
			m_enemyList.at(i)->Update(dTime);
		}
	}

	//CheckIndexRange();
	PlayerEnemyBulletCollision(dTime);
}

void EntityHandler::Release() {
	for (int i = 0; i < m_interactableList.size(); i++)
	{
		delete m_interactableList[i];
		m_interactableList.erase(m_interactableList.begin() + i);
		--i;
	}

	for (int i = 0; i < m_enemyList.size(); i++) {
		delete m_enemyList[i];
		m_enemyList.erase(m_enemyList.begin() + i);
		--i;
	}
}

void EntityHandler::Render() {
	for (int i = 0; i < m_interactableList.size(); i++) {
		if (m_interactableList.at(i)->GetActive()) {
			m_interactableList.at(i)->Render();
		}
	}

	for (int i = 0; i < m_enemyList.size(); i++) {
		if (m_enemyList.at(i)->GetActive()) {
			m_enemyList.at(i)->Render();
		}
	}
}

void EntityHandler::PlayerEnemyBulletCollision(float dTime) {
	for (int i = 0; i < m_enemyList.size(); i++) {
		if (m_enemyList.at(i)->GetActive() || m_enemyList.at(i)->getBullets().size() != 0) {
			//m_enemyList.at(i)->Update(dTime);
			
			for (size_t k = 0; k < m_enemyList.at(i)->getBullets().size(); ++k) {
				Vector3 cn;
				if (SphereToAABBox(BoundingBox(player->GetPos(), Vector3(0.25, 0.75, 02.5)), 
					BoundingSphere(m_enemyList.at(i)->getBullets().at(k)->pos, 0.25f), cn)) {
					player->takeDamage(20);
					m_enemyList.at(i)->getBullets().at(k)->bCollision = false;
				}
			}
		}
	}
}

void EntityHandler::SetAllAttributes() {
	// Set some default values
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_enemyList.at(i)->SetSpeed(0.5f);
		m_enemyList.at(i)->SetDir(Vector3(26.65, 0, 0));
		m_enemyList.at(i)->SetPos(Vector3(15, -8.5, 20));
		m_enemyList.at(i)->SetScale(Vector3(0.01, 0.01, 0.01));
		m_enemyList.at(i)->SetActive(true);
		m_enemyList.at(i)->SetHealth(100);
		m_enemyList.at(i)->SetAttackRange(10);
		m_enemyList.at(i)->SetDead(false);
		m_enemyList.at(i)->SetPlayer(player);
		m_enemyList.at(i)->SetTether(20);
		m_enemyList.at(i)->SetScaleFactor(1);
	}

	// Set unique attributes
	m_enemyList.at(0)->SetPos(Vector3(60, -8.5, 15));
	m_enemyList.at(0)->SetScale(Vector3(0.5, 0.5, 0.5));

	m_enemyList.at(1)->SetPos(Vector3(60, -8.5, 30));
	m_enemyList.at(1)->SetScaleFactor(2);
	m_enemyList.at(1)->SetHealth(150);
	m_enemyList.at(1)->SetTether(10);

	m_enemyList.at(2)->SetPos(Vector3(60, -8.5, 45));
	m_enemyList.at(2)->SetScale(Vector3(0.5, 0.5, 0.5));

	m_enemyList.at(3)->SetPos(Vector3(15, -8.5, 60));
	m_enemyList.at(3)->SetScale(Vector3(0.5, 0.5, 0.5));
	m_enemyList.at(3)->SetScaleFactor(3);
	m_enemyList.at(3)->SetHealth(250);
	m_enemyList.at(3)->SetTether(0);
	m_enemyList.at(3)->SetAttackRange(20);

	m_enemyList.at(4)->SetPos(Vector3(115, -8.5, 30));
	m_enemyList.at(4)->SetScaleFactor(3);
	m_enemyList.at(4)->SetHealth(750);
	m_enemyList.at(4)->SetTether(0);
	m_enemyList.at(4)->SetAttackRange(20);

	m_enemyList.at(5)->SetPos(Vector3(105, -8.5, 15));
	m_enemyList.at(5)->SetTether(5);
	m_enemyList.at(5)->SetAttackRange(10);

	m_enemyList.at(6)->SetPos(Vector3(120, -8.5, 50));
	m_enemyList.at(6)->SetTether(5);
	m_enemyList.at(6)->SetAttackRange(10);

	m_enemyList.at(7)->SetPos(Vector3(120, -8.5, 15));
	m_enemyList.at(7)->SetTether(5);
	m_enemyList.at(7)->SetAttackRange(10);

	m_enemyList.at(8)->SetPos(Vector3(105, -8.5, 50));
	m_enemyList.at(8)->SetTether(5);
	m_enemyList.at(8)->SetAttackRange(10);

	m_enemyList.at(9)->SetPos(Vector3(145, -8.5, 60));
	m_enemyList.at(9)->SetScaleFactor(4);
	m_enemyList.at(9)->SetTether(0);
	m_enemyList.at(9)->SetAttackRange(10);
	m_enemyList.at(9)->SetHealth(750);

	m_enemyList.at(10)->SetPos(Vector3(145, -8.5, 20));
	m_enemyList.at(10)->SetScale(Vector3(0.5, 0.5, 0.5));
	m_enemyList.at(10)->SetScaleFactor(3);
	m_enemyList.at(10)->SetTether(0);
	m_enemyList.at(10)->SetAttackRange(10);
	m_enemyList.at(10)->SetHealth(500);



	for (int i = 0; i < m_interactableList.size(); i++) {
		m_interactableList.at(i)->SetActive(true);
		m_interactableList.at(i)->SetTriggerSize(1.5f);
		m_interactableList.at(i)->SetRange(m_lightRange);
		m_interactableList.at(i)->SetIndex(4 + i);
	}

	m_interactableList.at(0)->SetPos(Vector3(145, -9, 55));
	m_interactableList.at(1)->SetPos(Vector3(145, -9, 25));
	m_interactableList.at(2)->SetPos(Vector3(89, -9, 15));
	m_interactableList.at(3)->SetPos(Vector3(15, -9, 60));
	m_interactableList.at(4)->SetPos(Vector3(60, -9, 50));
	m_interactableList.at(5)->SetPos(Vector3(140, -9, 25));
	m_interactableList.at(6)->SetPos(Vector3(145, -9, 60));
	m_interactableList.at(7)->SetPos(Vector3(115, -9, 30));
	m_interactableList.at(8)->SetPos(Vector3(60, -9, 30));
}

int EntityHandler::GetOpenIndex() {

	int index;

	if (m_lightIndxOpn.size() != 0) {
		int index = m_lightIndxOpn.front();
		m_lightIndxClsd.push_back(index);
		m_lightIndxOpn.erase(m_lightIndxOpn.begin());
		return index;
	}
	else {
		CheckIndexRange();
	}
}

void EntityHandler::CheckIndexRange() {
	for (int i = 0; i < m_interactableList.size(); i++) {

		float dist = Vector3::Distance(player->GetPos(), m_interactableList.at(i)->GetPos());

		if (dist > m_lightRange) {
			int index = m_interactableList.at(i)->GetIndex();

			for (int k = 0; k < m_lightIndxClsd.size(); k++) {
				if (m_lightIndxClsd.at(k) == index) {
					m_lightIndxClsd.erase(m_lightIndxClsd.begin() + k);
				}
			}
			m_lightIndxOpn.push_back(index);
		}
		else {
			if (m_lightIndxOpn.size() != 0) {
				int index = m_lightIndxOpn.front();
				m_lightIndxClsd.push_back(index);
				m_lightIndxOpn.erase(m_lightIndxOpn.begin());
				m_interactableList.at(i)->SetIndex(index);
			}
		}
	}
}

int EntityHandler::GetEntityUIState() {

	for (size_t i = 0; i < m_interactableList.size(); i++)
	{
		if (m_interactableList.at(i)->GetActive() && m_interactableList.at(i)->CanInteract())
			if (player->GetMouseLastInput())
				return 1;
			else
				return 2;
	}

	return 0;

}

int EntityHandler::GetObjectiveCount() {
	int objcount = 0;
	for (int i = 0; i < objectiveSize; i++) {
		if (m_interactableList.at(i)->GetActive() == false) {
			objcount++;
		}
	}
	return objcount;
}