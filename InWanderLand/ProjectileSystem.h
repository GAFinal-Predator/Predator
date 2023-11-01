#pragma once
#include "YunutyEngine.h"
#include <stack>
#include <vector>
/// <summary>
/// Unit�� �����ü�� �߻�ü �ý��� Ŭ����.
/// �ý����̶�� �̸��� �ɸ°� ������ �߻�ü ��� �� ��� �Լ��� ȣ��
/// �߻�ü�� �����̰� �浹�ϴ� ������ Projectile Ŭ�������� ��������.
/// </summary>

class Unit;
class Projectile;

class ProjectileSystem
{
private:
	ProjectileSystem();
	static ProjectileSystem* instance;

public:
	static ProjectileSystem* GetInstance();

private:
	std::vector<GameObject*> m_projectileVector;

	std::stack<GameObject*> m_projectileStack;								// �߻� ��� ���� ����ü��.
	std::unordered_map<GameObject*, Unit*> m_chasingProjectileMap;	// �߻� ���� ����ü�� �ش� ����ü�� �Ѿư� ��� ����.

public:
	void Shoot(Unit* ownerUnit, Unit* opponentUnit, float speed);
	void ReturnToPool(GameObject* usedObject);
	void SetUp();

};

