#pragma once
#include "YunutyEngine.h"
/// <summary>
/// UnitProductionOrder Ŭ������ ���� ������ ����ϰ�,
/// ������� ���ֿ� ��ã�� ������Ʈ�� �ٿ��ִ� Ŭ����.
/// �� �ʿ� ����
/// </summary>

class UnitProductionOrder;

class UnitFactory : public Component
{
public:
	void OrderCreateUnit(GameObject* orderedUnit, yunutyEngine::NavigationField* navField, Vector3d startPosition);
};

