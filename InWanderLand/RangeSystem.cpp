#include "RangeSystem.h"
#include "Unit.h"
#include "MoveDetector.h"

void RangeSystem::Start()
{

}

void RangeSystem::SetUnitComponent(Unit* unitComponent)
{
	m_unitComponent = unitComponent;
}

void RangeSystem::SetIdRadius(float radius)
{
	m_tempIDRadius = radius;
	SetRadius(radius);
}

void RangeSystem::SetAtkRadius(float radius)
{
	m_tempAtkRadius = radius;
}

void RangeSystem::OnCollisionEnter2D(const Collision2D& collision)
{
	/// �������� Ÿ�Կ� ���� Unit���� ȣ��� �Լ��� �����ش�.
	// �θ� ������Ʈ (����) �� Ÿ���� �ٸ��ٸ� �浹 ó��
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>() != nullptr && 
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>()->GetType())
	{	
		float distance = (collision.m_OtherCollider->GetGameObject()->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
		
		MoveDetector::GetInstance()->SetTargetUnit(collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>());
		MoveDetector::GetInstance()->SetChaserUnit(collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>(),
			m_unitComponent);

		m_unitComponent->SetIdRadius(m_tempIDRadius);
		m_unitComponent->SetAtkRadius(m_tempAtkRadius);
		
		m_unitComponent->SetOpponentGameObject(collision.m_OtherCollider->GetGameObject());
		m_unitComponent->EnterIDRange();
		m_unitComponent->IdleTransition();
	}
}

/// <summary>
/// �ν� ������ �ִٰ� ���� ���� ���. �� 
/// </summary>
/// <param name="collision"></param>
void RangeSystem::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>() != nullptr &&
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>()->GetType())
	{
		m_unitComponent->InitFSM();
	}
}

