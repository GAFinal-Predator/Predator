#pragma once
#include "YunutyEngine.h"
#include <stack>
/// <summary>
/// Unit�� �����ü�� �߻�ü �ý��� Ŭ����.
/// �ý����̶�� �̸��� �ɸ°� ������ �߻�ü ��� �� ��� �Լ��� ȣ��
/// �߻�ü�� �����̰� �浹�ϴ� ������ Projectile Ŭ�������� ��������.
/// </summary>

class Unit;
class Projectile;

class ProjectileSystem
{
public:
	ProjectileSystem(Unit* OwnerUnit);

private:
	Unit* m_ownerUnit;
	std::stack<Projectile*> projectileStack;

public:
	void SetProjectile(Projectile* p_projectile);
	void Shoot(Vector3d enemyPosition);
	void ReturnToStack(Projectile* usedProjectile);
	string GetUnitType() const;
	bool Empty() const;
};

