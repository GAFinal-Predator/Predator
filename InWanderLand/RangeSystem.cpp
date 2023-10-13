#include "RangeSystem.h"
#include "IRangeAction.h"

void RangeSystem::SetUnitGameObject(GameObject* obj)
{
	m_unit = obj;
	//this->GetGameObject()->SetParent(obj);
}

void RangeSystem::SetRangeType(string p_type)
{
	m_type = p_type;
}

void RangeSystem::OnCollisionEnter2D(const Collision2D& collision)
{
	/// �������� Ÿ�Կ� ���� Unit���� ȣ��� �Լ��� �����ش�.
	// �ӽ� ���� - �θ� ���ӿ�����Ʈ�� �ٸ� ��� �浹 �̺�Ʈ �߻�
	if (GetGameObject()->GetParentGameObject() != collision.m_OtherCollider->GetGameObject()->GetParentGameObject())
	{
		
		if (m_type == "Identification")
			m_unit->GetComponent<IRangeAction>()->IdentifyAction(collision.m_OtherCollider->GetTransform()->GetWorldPosition());

		else if (m_type == "Attack")
			m_unit->GetComponent<IRangeAction>()->AttackAction(collision.m_OtherCollider->GetTransform()->GetWorldPosition());

	}


}

