#include "UnitFactory.h"
#include "Unit.h"
#include "RangeSystem.h"

UnitFactory::UnitFactory()
{
	playerColor = yunuGI::Color{ 0, 1, 1, 0 };
	enemyColor = yunuGI::Color{ 1, 0, 1, 0 };

	defaultPlayerAtkRadius = 2.0f;
	defaultPlayerIDRadius = 3.0f;
	defaultPlayerSpeed = 2.5f;

	defaultEnemyAtkRadius = 1.0f;
	defaultEnemyIDRadius = 5.0f;
	defaultEnemySpeed = 1.0f;
}

yunutyEngine::GameObject* UnitFactory::CreateUnit(UnitType unitType, yunutyEngine::NavigationField* navField, Vector3d startPosition)
{
	/// 1. UnitGameObject ����
	auto unitGameObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	unitGameObject->GetTransform()->SetWorldPosition(startPosition);

	// 1-1. unitType�� ���� Unit ������Ʈ �߰�
	Unit* unitComponent = unitGameObject->AddComponent<Unit>();

	// 1-2. (�ӽ�) StaticMeshRenderer �߰�
	auto unitMesh = unitGameObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	unitMesh->GetGI().LoadMesh("Capsule");

	/// 2. RangeSystem Gameobject �߰�
	auto unitRangeSystemObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	unitRangeSystemObject->SetParent(unitGameObject);
	unitRangeSystemObject->GetTransform()->SetWorldRotation(Vector3d(90, 0, 0));

	// 2-1. RangeSystem Component �߰�
	RangeSystem* rangeSystemComponent = unitRangeSystemObject->AddComponent<RangeSystem>();

	// 2-2. RangeSystem Debug Mesh �߰�
	auto rangeSystemMesh = unitRangeSystemObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	rangeSystemMesh->GetGI().LoadMesh("Sphere");
	rangeSystemMesh->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");

	/// 3. NavigationAgent Component �߰�
	auto unitNavigationComponent = unitGameObject->AddComponent<NavigationAgent>();
	unitNavigationComponent->AssignToNavigationField(navField);

	/// 4. Collider Component �߰�
	auto unitColliderComponent = unitGameObject->AddComponent<CircleCollider2D>();
	//unitColliderComponent->SetRadius(1.0f);

	/// switch-case ������ Ŀ���͸���¡(?)
	switch (unitType)
	{
		case UnitType::PLAYER:
		{
			unitGameObject->setName("Player");
			unitComponent->SetType("Player");
			unitComponent->SetUnitHp(playerHp);
			unitComponent->SetUnitAp(playerAp);
			unitMesh->GetGI().GetMaterial()->SetColor(playerColor);
			unitComponent->SetAtkRadius(defaultPlayerAtkRadius);
			unitComponent->SetIdRadius(defaultPlayerIDRadius);
			unitComponent->SetUnitSpeed(defaultPlayerSpeed);
			
			rangeSystemComponent->SetRadius(defaultPlayerIDRadius);

			rangeSystemMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });		// �� �ٸ��� �ҰŸ� switch-case�ȿ� �ֱ�.

			unitRangeSystemObject->GetTransform()->scale = Vector2d(defaultPlayerIDRadius * 2, defaultPlayerIDRadius * 2);

			unitRangeSystemObject->setName("PlayerRangeObject");
			break;
		}

		case UnitType::ENEMY:
		{
			unitGameObject->setName("Enemy");
			unitComponent->SetType("Enemy");
			unitComponent->SetUnitHp(enemyHp);
			unitComponent->SetUnitAp(enemyAp);
			unitMesh->GetGI().GetMaterial()->SetColor(enemyColor);
			unitComponent->SetAtkRadius(defaultEnemyAtkRadius);
			unitComponent->SetIdRadius(defaultEnemyIDRadius);
			unitComponent->SetUnitSpeed(defaultEnemySpeed);

			rangeSystemComponent->SetRadius(defaultEnemyIDRadius);

			rangeSystemMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });		// �� �ٸ��� �ҰŸ� switch-case�ȿ� �ֱ�.

			unitRangeSystemObject->GetTransform()->scale = Vector2d(defaultEnemyIDRadius * 2, defaultEnemyIDRadius * 2);

			unitRangeSystemObject->setName("EnemyRangeObject");
			break;
		}
	}
	rangeSystemComponent->SetOwnerUnitComponent(unitComponent);


	return unitGameObject;
}

void UnitFactory::SetIDRadius(float idRadius)
{

}

void UnitFactory::SetAtkRadius(float atkRadius)
{

}

void UnitFactory::SetPlayerHp(int p_hp)
{
	playerHp = p_hp;
}

void UnitFactory::SetPlayerAp(int p_ap)
{
	playerAp = p_ap;
}

void UnitFactory::SetEnemyHp(int p_hp)
{
	enemyHp = p_hp;
}

void UnitFactory::SetEnemyAp(int p_ap)
{
	enemyAp = p_ap;
}
