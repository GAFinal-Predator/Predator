#pragma once
#include "YunutyEngine.h"
/// <summary>
/// UnitProductionOrder Ŭ������ ���� ������ ����ϰ�,
/// ������� ���ֿ� ��ã�� ������Ʈ�� �ٿ��ִ� Ŭ����.
/// �� �ʿ� ����
/// </summary>

class UnitFactory
{
public:
	void OrderCreateUnit(yunutyEngine::GameObject* orderedUnit, yunutyEngine::NavigationField* navField, Vector3d startPosition);
};

