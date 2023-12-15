#include "RangeSystem.h"
#include "Unit.h"

void RangeSystem::Start()
{

}

void RangeSystem::SetOwnerUnitComponent(Unit* unitComponent)
{
	m_unitComponent = unitComponent;
}


void RangeSystem::OnCollisionEnter2D(const Collision2D& collision)
{
	/// �������� Ÿ�Կ� ���� Unit���� ȣ��� �Լ��� �����ش�.
	// �θ� ������Ʈ (����) �� Ÿ���� �ٸ��ٸ� �浹 ó��
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>() != nullptr &&
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>()->GetType())
	{	
		float distance = (collision.m_OtherCollider->GetGameObject()->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
		
		m_unitComponent->AddToOpponentObjectList(collision.m_OtherCollider->GetGameObject());
		//m_unitComponent->IdleTransition();
	}
}

/// <summary>
/// �ν� ������ �ִٰ� ���� ���� ���. 
/// </summary>
/// <param name="collision"></param>
void RangeSystem::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>() != nullptr &&
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>()->GetType())
	{
		m_unitComponent->DeleteFromOpponentObjectList(collision.m_OtherCollider->GetGameObject());
	}
}

