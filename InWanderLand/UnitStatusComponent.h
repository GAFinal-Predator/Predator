#pragma once
#include "YunutyEngine.h"
/// <summary>
/// ������ ���� �����̻�� ���� ���� ������Ʈ.
/// </summary>

class Unit;

class UnitStatusComponent : public Component
{
private:
	Unit* m_ownerUnit;
	Unit* m_currentOpponentUnit;

public:
	virtual void SetOwnerUnit(Unit* ownerUnit);
	virtual void ApplyStatus(Unit* ownerUnit, Unit* opponentUnit) = 0;
};

