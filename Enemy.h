// Enemy base class
#ifndef EnemyH
#define EnemyH

#include <vector>
#include "Mesh.h"
#include "Model.h"
#include "PlayerController.h" // acts as the player
#include "SimpleMath.h"
#include "bullet.h"
#include "FX.h"
#include "Level.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Enemy {
public:
	Enemy();
	virtual ~Enemy() {
		Release();
	};

	virtual void Initialise();
	virtual void Update(float dTime);
	virtual void Render();
	virtual void Release();
	virtual void Attack(float dTime, Vector3 pos);
	virtual void Die();
	virtual vector<Bullet*> getBullets() { return bullets; }
	// Return the movement goal of the enemy
	Vector3 GetMovePos(float dTime, Vector3 target);
	float	GetRotPos(Vector3 target);

	// Helpers
	void	SetTargetPos(Vector3 m_newTarget);
	Vector3 GetTargetPos();

	void	SetSpeed(float newSpeed);
	float	GetSpeed();

	void	SetDir(Vector3 newDir);
	Vector3 GetDir();

	void	SetPos(Vector3 getPos);
	Vector3 GetPos();

	void	SetScale(Vector3 newScale);
	Vector3 GetScale();

	void	SetHealth(float newHealth);
	float	GetHealth();

	void	SetRange(float newRange);
	float	GetRange();

	void	SetAttackRange(float newRange);
	float	GetAttackRange();

	void	SetDead(bool isDead);
	bool	GetDead();

	void	TakeDamage(float amt);

	void	SetActive(bool newActive);
	bool	GetActive();

	void	SetTether(float newTeather);
	float	GetTether();

	void	SetStartPos(Vector3 newStartPos);
	Vector3 GetStartPos();

	void	SetScaleFactor(int newScale);
	int		GetScaleFactor();

	float	GetDist(Vector3 a, Vector3 b);
	float	DistToTarget();
	void	CheckCollision();

	void SetPlayer(PlayerController* newPlayer);
	vector<Bullet*>bullets;

	
protected:
private:
	PlayerController * m_Player;
	Vector3 targetPos;

	// Helper varaibles
	float	m_speed;
	Vector3 m_dir;
	Vector3 m_pos;
	Vector3 m_scale;
	float	m_RotY;
	float   m_range;
	float	m_attackRange;
	bool	m_active;
	float	m_health;
	bool	m_dead;
	float	m_tether;
	Vector3 m_startPos;
	int		m_scaleFactor;

	float distanceToTarget;
};
#endif // !EnemyH

