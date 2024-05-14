#pragma once
#include "YunutyEngine.h"
#include <map>
#include "Unit.h"
#include "PermanentObservee.h"
/// <summary>
/// UnitFactory에서 Player를 만들 경우, PlayerController에 해당 Player를 등록한다.
/// 고유 번호를 배분하여 키입력으로 지정할 수 있도록 해보자!
/// </summary>

class RTSCam;
class Unit;
class CursorDetector;

class PlayerController : public Component, public SingletonComponent<PlayerController>, public PermanentObservee
{
public:
	enum class OrderType
	{
		Move,
		AttackMove,
		QSkill,
	};

private:
	RTSCam* m_movingSystemComponent;
	Dotween* m_dotween;
	std::unordered_map<Unit::UnitType, Unit*> playerComponentMap;
	Unit::UnitType currentSelectedSerialNumber = Unit::UnitType::Warrior;
	int previousSerialNumber = 0;

	Vector3d cameraOffset = { 0, 20, -15 };
	float cameraMoveDuration{ 0.3f };

	void ChangeLeaderPlayerUnit(Unit::UnitType p_num);

public:
	float lookRotationDuration = 0.1f;

	//void SelectFunctionByOrderType(int unitSerialNumber, OrderType p_orderType);
public:
	virtual void Start() override;

	void SetMovingSystemComponent(RTSCam* sys);
	void AddPlayerUnit(Unit* p_playerUnit);
	void ErasePlayerUnit(Unit* p_playerUnit);

	virtual void PlayFunction() override;
	virtual void StopFunction() override;

	void SetRightClickFunction();
	void SetLeftClickAttackMove();
	void SetLeftClickSkill(Unit::SkillEnum p_skillNum);

	void SetLeftClickEmpty();
	void SetRightClickEmpty();

	void SetCurrentPlayerSerialNumber(Unit::UnitType p_num);
	void ReportBattleEnded();

	std::unordered_map<Unit::UnitType, Unit*> GetPlayerMap() const;
	Unit* FindSelectedUnitByUnitType(Unit::UnitType p_type);
	Unit* GetCurrentSelectedPlayerUnit() const;
	CursorDetector* m_cursorDetector;
};

