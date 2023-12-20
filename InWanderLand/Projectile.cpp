#include "Projectile.h"
#include "Dotween.h"
#include "ProjectileSystem.h"
#include "Unit.h"

void Projectile::SetOwnerType(Unit::UnitType type)
{
	ownerType = type;
}

void Projectile::Shoot(Unit* ownerUnit, Unit* opponentUnit, float speed)
{
	m_speed = speed;
	m_ownerUnit = ownerUnit;
	m_opponentUnit = opponentUnit;
	GetGameObject()->GetTransform()->SetWorldPosition(ownerUnit->GetGameObject()->GetTransform()->GetWorldPosition());

	isShootStarted = true;
}

void Projectile::ShootFunction()
{
	//// �����̱� ���� ����ü ��ġ
	Vector3d startPosition = GetGameObject()->GetTransform()->GetWorldPosition();
	Vector3d endPosition = m_opponentUnit->GetGameObject()->GetTransform()->GetWorldPosition();
	float distance = (endPosition - startPosition).Magnitude();

	// �����̱� �� ���� ����
	Vector3d directionVector = (endPosition - startPosition).Normalized();

	// �����̰� ������ ����ü ��ġ.
	Vector3d movedPositionPerFrame = GetGameObject()->GetTransform()->GetWorldPosition() + (directionVector * m_speed * Time::GetDeltaTime());
	
	// �����̰� ������ ����ü -> ��ǥ�� ���� ����
	Vector3d afterDirectionVector = (endPosition - movedPositionPerFrame).Normalized();

	GetGameObject()->GetTransform()->SetWorldRotation(directionVector);
	GetGameObject()->GetTransform()->SetWorldPosition(movedPositionPerFrame);

	float dotProducted = Vector3d::Dot(directionVector, afterDirectionVector);
	float multipledLength = directionVector.Magnitude() * afterDirectionVector.Magnitude();

	float angle = dotProducted / multipledLength;

	float finalAngle = acos(angle);

	if (angle < 0)
	{
		GetGameObject()->SetSelfActive(false);
		ProjectileSystem::GetInstance()->ReturnToPool(GetGameObject());

		m_opponentUnit->Damaged(m_ownerUnit->GetGameObject(), m_ownerUnit->GetUnitAp());

		isShootStarted = false;
	}
}

void Projectile::Start()
{

}

void Projectile::Update()
{
	if (isShootStarted)
	{
		ShootFunction();
	}
}

