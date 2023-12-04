#pragma once
#include "YunutyEngine.h"
#include <stack>
#include <vector>
/// <summary>
/// Unit�� �����ü�� �߻�ü �ý��� Ŭ����.
/// �ý����̶�� �̸��� �ɸ°� ������ �߻�ü ��� �� ��� �Լ��� ȣ��
/// �߻�ü�� �����̰� �浹�ϴ� ������ Projectile Ŭ�������� ��������.
/// </summary>

class BaseUnitEntity;
class Projectile;

class ProjectileSystem
{
private:
	ProjectileSystem();
	static ProjectileSystem* instance;

public:
	static ProjectileSystem* GetInstance();

private:
	std::stack<GameObject*> m_projectileStack;								// �߻� ��� ���� ����ü��.

public:
	void Shoot(BaseUnitEntity* ownerUnit, BaseUnitEntity* opponentUnit, float speed);
	void ReturnToPool(GameObject* usedObject);
	void SetUp();

};

