#ifndef HealthPickupH
#define HealthPickupH

#include "Interactable.h"

class HealthPickup : public Interactable
{
public:
	HealthPickup(PlayerController *plyaer);
	~HealthPickup() {
		Release();
	}

	void Initialise();
	void Update(float dTime);
	void Release();
	void Render();
	void Interact();
private:
	Model mModel;
	PlayerController* m_Player;
	Gamepad m_Pad;
	int m_HealthToAdd;
};
#endif // !HealthPickup
