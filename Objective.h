#ifndef ObjectiveH
#define ObjectiveH

#include "Interactable.h"

class Objective : public Interactable {
public:
	Objective(PlayerController *player, wstring msg);
	~Objective() {
		Release();
	};

	void Initialise();
	void Update(float dTime);
	void Release();
	void Render();
	void Interact();
	wstring getMsg() const { return message; }
	bool GetObjectiveClear();
private:
	Model mModel;
	PlayerController* m_Player;
	Gamepad m_Pad;
	wstring message;

	bool m_ObjectiveClear;
};

#endif