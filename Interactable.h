#ifndef InteractableH
#define InteractableH

#include "D3D.h"
#include "Model.h"
#include "PlayerController.h"
#include "FX.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Interactable {
public:
	Interactable();
	virtual ~Interactable() {
		Release();
	}

	bool CanInteract();
	virtual void Render();
	virtual void Interact();
	virtual void Release();
	virtual void Initialise();
	virtual void Update(float dTime);

	void SetPos(Vector3 newPos);
	Vector3 GetPos();

	void SetTriggerSize(float newTriggerSize);
	float GetTriggerSize();

	void SetActive(bool isActive);
	bool GetActive();

	void SetIndex(int newIndex);
	int GetIndex();

	void SetRange(float newRange);
	float GetRange();

	bool GetWithinLightRange();

	void SetPlayer(PlayerController *newPlayer);
private:
	Vector3 m_Pos;
	float m_TriggerSize;
	bool m_isActive;
	PlayerController *m_Player;
	int m_lightIndex;
	float m_activeRange;
};

#endif
