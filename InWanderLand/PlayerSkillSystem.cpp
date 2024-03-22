#include "InWanderLand.h"
#include "PlayerSkillSystem.h"
#include "Unit.h"

void PlayerSkillSystem::SkillActivate(Unit::SkillEnum p_currentSkill, Vector3d p_skillPosition)
{
	switch (p_currentSkill)
	{
		case Unit::SkillEnum::Q:
			QSkillActivate(p_skillPosition);
			break;
		case Unit::SkillEnum::W:
			WSkillActivate(p_skillPosition);
			break;
		default:
			break;
	}
}

bool PlayerSkillSystem::IsSkillEnd() const
{
	return isSkillEnd;
}

void PlayerSkillSystem::RotateProjectile(GameObject* p_projectileObject, Vector3d p_endPosition)
{
	Vector3d startPosition = p_projectileObject->GetTransform()->GetWorldPosition();
	Vector3d objectFront = p_projectileObject->GetTransform()->GetWorldRotation().Forward();
	Vector3d distanceVec = p_endPosition - startPosition;

	double dot = Vector3d::Dot(objectFront, startPosition - p_endPosition);

	double angle;
	double sq;
	double finalAngle;
	double finalDegree;

	// 회전 방향 판정
	Vector3d axis = Vector3d::Cross(objectFront, distanceVec);

	angle = (objectFront.x * distanceVec.x + objectFront.z * distanceVec.z);
	sq = (sqrt(pow(objectFront.x, 2) + pow(objectFront.z, 2)) *
		sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));

	// 두 벡터의 각도가 180도 이상이면 180을, -180 이하 이라면 -180을 
	//finalAngle = acos( max( -1.0f, min(1.0f, angle / sq) ) );
	finalAngle = acos(std::clamp(angle / sq, -1.0, 1.0));			// c++17 된다면
	finalDegree = 57.2969f * (finalAngle);

	if (axis.y < 0)
		finalDegree *= -1;

	if (abs(finalDegree) > 0.05)
		p_projectileObject->GetTransform()->SetWorldRotation(Quaternion({ 0.0f, finalDegree, 0.0f }));
}

void PlayerSkillSystem::SetOtherComponentsAsMember()
{
	m_unitComponent = GetGameObject()->GetComponent<Unit>();
	m_unitDotween = GetGameObject()->GetComponent<Dotween>();
	m_unitNavComponent = GetGameObject()->GetComponent<NavigationAgent>();
}
