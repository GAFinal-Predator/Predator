#pragma once
#include "UnitProductionOrder.h"

class MagicianProduction : public UnitProductionOrder
{
public:
	virtual void SetUnitData(GameObject* fbxObject) override;
	virtual GameObject* CreateUnitWithOrder() override;
};

