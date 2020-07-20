#include "Interactable.h"

Interactable::Interactable() {

}

bool Interactable::CanInteract() {
	float distance = Vector3::Distance(m_Player->GetPos(), m_Pos);

	if (distance <= m_TriggerSize)
		return true;

	return false;
}

bool Interactable::GetWithinLightRange() {
	float distance = Vector3::Distance(m_Player->GetPos(), m_Pos);

	if (distance > m_activeRange)
		return false;

	return true;
}

void Interactable::Interact() {}

void Interactable::Render() {}

void Interactable::Release() {}

void Interactable::Initialise() {}

void Interactable::Update(float dTime) {}

void	Interactable::SetPos(Vector3 newPos) { m_Pos = newPos; }
Vector3 Interactable::GetPos() { return m_Pos; }

void	Interactable::SetTriggerSize(float newTriggerSize) { m_TriggerSize = newTriggerSize; }
float	Interactable::GetTriggerSize() { return m_TriggerSize; }

void	Interactable::SetActive(bool isActive) { m_isActive = isActive; }
bool	Interactable::GetActive() { return m_isActive; }

void	Interactable::SetIndex(int newIndex) { m_lightIndex = newIndex; }
int		Interactable::GetIndex() { return m_lightIndex; }

void	Interactable::SetRange(float newRange) { m_activeRange = newRange; }
float	Interactable::GetRange() { return m_activeRange; }

void	Interactable::SetPlayer(PlayerController *newPlayer) { m_Player = newPlayer; }