#pragma once
#include "YunutyEngine.h"
#include <stack>
#include <vector>
/// <summary>
/// Unit의 멤버객체인 발사체 시스템 클래스.
/// 시스템이라는 이름에 걸맞게 역할은 발사체 등록 및 사격 함수만 호출
/// 발사체가 움직이고 충돌하는 로직은 Projectile 클래스에서 관리하자.
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

	std::stack<GameObject*> m_projectileStack;								// 발사 대기 중인 투사체들.
	std::unordered_map<GameObject*, Unit*> m_chasingProjectileMap;	// 발사 후의 투사체와 해당 투사체가 쫓아갈 대상 유닛.

public:
	void Shoot(Unit* ownerUnit, Unit* opponentUnit, float speed);
	void ReturnToPool(GameObject* usedObject);
	void SetUp();

};

