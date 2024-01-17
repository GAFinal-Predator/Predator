#pragma once
#include "UnitProductionOrder.h"

class MagicianProduction : public UnitProductionOrder
{
private:
	float m_QSkillProjectileRadius;
	float m_QSkillFieldRadius;
public:
	virtual void SetUnitData(GameObject* fbxObject, NavigationField* navField, Vector3d startPosition) override;
	virtual GameObject* CreateUnitWithOrder() override;
};

