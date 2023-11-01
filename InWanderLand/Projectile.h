#pragma once
#include "YunutyEngine.h"
/// <summary>
/// ������ �߻�ü�� �ٿ��� ������Ʈ.
/// �����Ӱ� �ٸ� ���ְ� �浹 ���� ����� �������� �����ȴ�.
/// </summary>

class Unit;

class Projectile : public Component
{
private:
	float m_speed;
	string ownerType;
	Unit* m_opponentUnit;
	Unit* m_ownerUnit;

	bool isShootStarted = false;
	bool isPassed;

public:
	void SetOwnerType(string type);

	void Shoot(Unit* ownerUnit, Unit* opponentUnit, float speed);
	void ShootFunction();

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

