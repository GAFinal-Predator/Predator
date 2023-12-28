#pragma once
#include "UnitStatusComponent.h"

class StatusTimer;

class BleedingComponent : public UnitStatusComponent
{
private:
	int m_maxDamagedCount = 3;			// ��ǥ ���� ������ Ƚ��
	int m_currentDamagedCount = 0;			// ������ �������� ���� Ƚ��
	int m_bleedDamage = 1.0f;
	float m_bleedDuration = 5.0f;			// ���� ���� �ð�
	float m_bleedInterval = 1.0f;			// ���� ���� ����
	float m_bleedElapsed = 0.0f;			// ���� ���� ��� �ð�

	std::map<Unit*, StatusTimer*> opponentUnitMap;	// ���� ������ �޴� ����, �ش� ������ ���� Ÿ�̸�

public:
	virtual void ApplyStatus(Unit* ownerUnit, Unit* opponentUnit) override;
	
public:
	virtual void Update() override;
};

