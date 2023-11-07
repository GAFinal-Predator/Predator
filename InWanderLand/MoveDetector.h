#pragma once
#include "YunutyEngine.h"
#include <map>
#include <vector>
/// <summary>
/// ������ �ڿ������� ������ ������ ���� ���� �̵� ���� Ŭ����.
/// ������ ������ ������ ������ �����ϰ� �ִ� �ٸ� ��ü���� �Ϸ���ġ�� ����.
/// 
/// </summary>

class Unit;

class MoveDetector
{	
public:
	MoveDetector();
	static MoveDetector* instance;

public:
	static MoveDetector* GetInstance();

private:
	std::map<Unit*, std::vector<Unit*>> m_unitMap;				// First : Target	Second : Chasers

public:
	void SetTargetUnit(Unit* target);
	void SetChaserUnit(Unit* target, Unit* chaser);
	void TargetMove(Unit* target, Vector3d endPosition);					// target�� �����̸� �ش� �Լ��� ȣ�����ش�.

};

