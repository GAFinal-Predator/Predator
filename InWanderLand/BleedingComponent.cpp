#include "BleedingComponent.h"
#include "StatusTimerPool.h"
#include "StatusTimer.h"
#include "Unit.h"

void BleedingComponent::ApplyStatus(Unit* ownerUnit, Unit* opponentUnit)
{
	/// ���� �ý���	
	/// damage�� �ִ°� Unit->Damaged �Լ��� ȣ��,
	/// �̿��� ����(���� ����, ���� ����, )�� ���ο��� ����
	
	StatusTimer* bleedingTimer = StatusTimerPool::GetInstance()->GetStatusTimer();
	opponentUnitMap.insert({ opponentUnit, bleedingTimer });

	bleedingTimer->m_isRepeated = true;
	bleedingTimer->m_duration = m_bleedDuration;
	bleedingTimer->onExpiration = [=]()
	{
		if (m_currentDamagedCount == m_maxDamagedCount)
		{
			m_currentDamagedCount = 0;
			bleedingTimer->StopTimer();
			opponentUnitMap.erase(opponentUnit);
			StatusTimerPool::GetInstance()->ReturnToPool(bleedingTimer);
		}
		else
		{
			opponentUnit->Damaged(m_bleedDamage);
			m_currentDamagedCount++;
		}
	};

	bleedingTimer->ActivateTimer();
}

void BleedingComponent::Update()
{
	
}
