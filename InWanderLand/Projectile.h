#pragma once
#include "YunutyEngine.h"
/// <summary>
/// ������ �߻�ü�� �ٿ��� ������Ʈ.
/// �����Ӱ� �ٸ� ���ְ� �浹 ���� ����� �������� �����ȴ�.
/// </summary>

class ProjectileSystem;

class Projectile : public Component
{
private:
	float m_speed = 2.0f;
	bool isHit;

	ProjectileSystem* m_projectileSystem;

public:
	void Shoot(Vector3d endPosition);
	
	void SetSystem(ProjectileSystem* sys);

public:
	virtual void Start() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

