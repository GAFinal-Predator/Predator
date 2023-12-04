#include "RangeSystem.h"
#include "Enemy.h"

void RangeSystem::Start()
{

}

void RangeSystem::SetOwnerUnitComponent(BaseUnitEntity* unitComponent)
{
	m_unitComponent = unitComponent;
}


void RangeSystem::OnCollisionEnter2D(const Collision2D& collision)
{
	/// �������� Ÿ�Կ� ���� Unit���� ȣ��� �Լ��� �����ش�.
	// �θ� ������Ʈ (����) �� Ÿ���� �ٸ��ٸ� �浹 ó��
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<BaseUnitEntity>() != nullptr &&
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<BaseUnitEntity>()->GetType())
	{	
		float distance = (collision.m_OtherCollider->GetGameObject()->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
		
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
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<BaseUnitEntity>() != nullptr &&
		m_unitComponent->GetType() != collision.m_OtherCollider->GetGameObject()->GetComponent<BaseUnitEntity>()->GetType())
	{
		m_unitComponent->InitFSM();
		m_unitComponent->DeleteOpponentGameObject(collision.m_OtherCollider->GetGameObject());
	}
}

