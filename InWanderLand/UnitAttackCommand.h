#pragma once
#include "YunutyEngine.h"

#include "UnitCommand.h"
#include "TacticModeSystem.h"
class Unit;
class yunuGI::IMesh;

class UnitAttackCommand : public UnitCommand
{
public:
	UnitAttackCommand(Unit* unit, Vector3d expectedPos, Unit* enemyUnit, bool isAttackAfterMove);
	virtual ~UnitAttackCommand();

public:
	virtual void Execute() override;
	virtual void ShowPreviewMesh() override;
	virtual void HidePreviewMesh() override;

private:
	yunutyEngine::graphics::StaticMeshRenderer* renderer;
	Unit* enemyUnit;

};

