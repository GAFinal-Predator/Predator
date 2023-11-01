#include "Projectile.h"
#include "Dotween.h"
#include "ProjectileSystem.h"
#include "Unit.h"

void Projectile::SetOwnerType(string type)
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
	//// 움직이기 전의 투사체 위치
	Vector3d startPosition = GetGameObject()->GetTransform()->GetWorldPosition();
	Vector3d endPosition = m_opponentUnit->GetGameObject()->GetTransform()->GetWorldPosition();
	float distance = (endPosition - startPosition).Magnitude();

	// 움직이기 전 방향 벡터
	Vector3d directionVector = (endPosition - startPosition).Normalized();

	// 움직이고 나서의 투사체 위치.
	Vector3d movedPositionPerFrame = GetGameObject()->GetTransform()->GetWorldPosition() + (directionVector * m_speed * Time::GetDeltaTime());
	
	// 움직이고 나서의 투사체 -> 목표물 방향 벡터
	Vector3d afterDirectionVector = (endPosition - movedPositionPerFrame).Normalized();

	GetGameObject()->GetTransform()->SetWorldRotation(directionVector);
	GetGameObject()->GetTransform()->SetWorldPosition(movedPositionPerFrame);

	/// 투사체가 한번 자리를 옮겼다. 그 후 목표 유닛에 부딫혔는지 판단하고,
	/// 부딫혔다면 Pool에 리턴해주고, 아니라면 재귀한다.
	/// 부딫혔는지를 어떻게 판단할 것인가? -> 
	float dotProducted = Vector3d::Dot(directionVector, afterDirectionVector);
	float multipledLength = directionVector.Magnitude() * afterDirectionVector.Magnitude();

	float angle = dotProducted / multipledLength;

	float finalAngle = acos(angle);

	if (angle < 0)
	{
		GetGameObject()->SetSelfActive(false);
		ProjectileSystem::GetInstance()->ReturnToPool(GetGameObject());
		isShootStarted = false;
	}
}

void Projectile::Start()
{
	isPassed = false;
}

void Projectile::Update()
{
	if (isShootStarted)
	{
		ShootFunction();
	}
}

void Projectile::OnCollisionEnter2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>() != nullptr && 
		collision.m_OtherCollider->GetGameObject()->GetComponent<Unit>()->GetType() != ownerType)
	{
		ProjectileSystem::GetInstance()->ReturnToPool(collision.m_Collider->GetGameObject());
	}
}
