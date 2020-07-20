#ifndef AmmoPickupH
#define AmmoPickupH

#include "Interactable.h"

class AmmoPickup : public Interactable
{
public:
	AmmoPickup(PlayerController *player);
	~AmmoPickup() {
		Release();
	};

	void Initialise();
	void Update(float dTime);
	void Release();
	void Render();
	void Interact();
private:
	Model mModel;
	PlayerController* m_Player;
	Gamepad m_Pad;
	int m_MagsToAdd;
};
#endif
