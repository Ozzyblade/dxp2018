// This script handle the enemy behaviour
#include "Enemy.h"
#include "MainMode.h"
#include "Mesh.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


Enemy::Enemy()
{

}

Vector3 Enemy::GetMovePos(float dTime, Vector3 target) {
	Vector3 newPos = (target - m_pos) * m_speed * dTime;
	return newPos;
}

float Enemy::GetRotPos(Vector3 target) {

	Vector3 worldForward = Vector3(0, 0, 1);
	Vector3 lookDirection;
	Vector3 adjTargetPos = Vector3(target.x, 0, target.z);
	lookDirection = target - m_pos;

	lookDirection.Normalize();

	m_RotY = acos(lookDirection.Dot(worldForward));
	if (lookDirection.x < 0 && m_RotY > 0)
		m_RotY *= -1;

	return m_RotY;
}



float Enemy::GetDist(Vector3 a, Vector3 b) {
	return Vector3::Distance(a, b);
}

void Enemy::TakeDamage(float amt) {
	SetHealth(GetHealth() - amt);

	if (GetHealth() <= 0) {
		Die();
		SetDead(true);
	}
}

void Enemy::CheckCollision() {
	for (size_t i = 0; i < m_Player->pBullets.size(); ++i) {

		if (m_active) {
			Vector3 cn;

			if (SphereToSphere(BoundingSphere(m_Player->pBullets.at(i)->pos, 0.25), BoundingSphere(m_pos, (0.75 * GetScaleFactor())), cn)) {
				m_Player->pBullets.at(i)->bCollision = false;
				TakeDamage(20);
			}
		}
	}
}

void Enemy::Initialise() {}

void Enemy::Update(float dTime) {}

void Enemy::Attack(float dTime, Vector3 pos) {}

void Enemy::Die() {}

void Enemy::Render() {}

void Enemy::Release() {}

void	Enemy::SetPlayer(PlayerController * newPlayer) { m_Player = newPlayer; }

void	Enemy::SetTargetPos(Vector3 newTarget)	{ targetPos = newTarget; }
Vector3 Enemy::GetTargetPos()					{ return targetPos; }

void	Enemy::SetSpeed(float newSpeed)			{ m_speed = newSpeed; }
float	Enemy::GetSpeed()						{ return m_speed; }

void	Enemy::SetDir(Vector3 newDir)			{ m_dir = newDir; }
Vector3 Enemy::GetDir()							{ return m_dir; }

void	Enemy::SetPos(Vector3 newPos)			{ m_pos = newPos; }
Vector3 Enemy::GetPos()							{ return m_pos; }

void	Enemy::SetScale(Vector3 newScale)		{ m_scale = newScale; }
Vector3	Enemy::GetScale()						{ return m_scale; }

void	Enemy::SetActive(bool newActive)		{ m_active = newActive; }
bool	Enemy::GetActive()						{ return m_active; }

void	Enemy::SetHealth(float newHealth)		{ m_health = newHealth; }
float	Enemy::GetHealth()						{ return m_health; }

void	Enemy::SetRange(float newRange)			{ m_range = newRange; }
float	Enemy::GetRange()						{ return m_range; }

void	Enemy::SetAttackRange(float newRange)	{ m_attackRange = newRange; }
float	Enemy::GetAttackRange()					{ return m_attackRange; }

void	Enemy::SetDead(bool isDead)				{ m_dead = isDead; }
bool	Enemy::GetDead()						{ return m_dead; }

void	Enemy::SetTether(float newTether)		{ m_tether = newTether; }
float	Enemy::GetTether()						{ return m_tether; }

void	Enemy::SetStartPos(Vector3 newStartPos) { m_startPos = newStartPos; }
Vector3 Enemy::GetStartPos()					{ return m_startPos; }

void	Enemy::SetScaleFactor(int newFactor)	{ m_scaleFactor = newFactor; }
int		Enemy::GetScaleFactor()					{ return m_scaleFactor; }
	
float	Enemy::DistToTarget()					{ return distanceToTarget; }