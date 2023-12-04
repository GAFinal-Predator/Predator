#pragma once
#include "YunutyEngine.h"
/// <summary>
/// ������ �߻�ü�� �ٿ��� ������Ʈ.
/// �����Ӱ� �ٸ� ���ְ� �浹 ���� ����� �������� �����ȴ�.
/// </summary>

class BaseUnitEntity;

class Projectile : public Component
{
private:
	float m_speed;
	string ownerType;
	BaseUnitEntity* m_opponentUnit;
	BaseUnitEntity* m_ownerUnit;

	bool isShootStarted = false;
	bool isPassed;

public:
	void SetOwnerType(string type);

	void Shoot(BaseUnitEntity* ownerUnit, BaseUnitEntity* opponentUnit, float speed);
	void ShootFunction();

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

