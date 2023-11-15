#include "UnitFactory.h"
#include "Unit.h"
#include "Player.h"
#include "RangeSystem.h"
#include "Dotween.h"

UnitFactory::UnitFactory()
{
	playerColor = yunuGI::Color{ 0, 1, 1, 0 };
	enemyColor = yunuGI::Color{ 1, 0, 1, 0 };

	defaultPlayerAtkRadius = 2.0f;
	defaultPlayerIDRadius = 3.0f;

	defaultEnemyAtkRadius = 3.0f;
	defaultEnemyIDRadius = 5.0f;
}

yunutyEngine::GameObject* UnitFactory::CreateUnit(UnitType unitType, yunutyEngine::NavigationField* navField)
{
	/// 1. UnitGameObject ����
	auto unitGameObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	unitGameObject->AddComponent<Dotween>();

	// 1-1. unitType�� ���� Unit ������Ʈ �߰�
	Unit* unitComponent;

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

	/// switch-case ������ Ŀ���͸���¡(?)
	switch (unitType)
	{
		case UnitType::PLAYER:
		{
			unitGameObject->setName("Player");
			unitComponent = unitGameObject->AddComponent<Player>();
			unitMesh->GetGI().GetMaterial()->SetColor(playerColor);
			rangeSystemComponent->SetAtkRadius(defaultPlayerAtkRadius);
			rangeSystemComponent->SetIdRadius(defaultPlayerIDRadius);
			rangeSystemMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });		// �� �ٸ��� �ҰŸ� switch-case�ȿ� �ֱ�.

			unitRangeSystemObject->GetTransform()->scale = Vector2d(defaultPlayerIDRadius * 2, defaultPlayerIDRadius * 2);
			break;
		}

		case UnitType::ENEMY:
		{
			unitGameObject->setName("enemyUnit");
			unitComponent = unitGameObject->AddComponent<Unit>();
			unitMesh->GetGI().GetMaterial()->SetColor(enemyColor);
			rangeSystemComponent->SetAtkRadius(defaultEnemyAtkRadius);
			rangeSystemComponent->SetIdRadius(defaultEnemyIDRadius);
			rangeSystemMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });		// �� �ٸ��� �ҰŸ� switch-case�ȿ� �ֱ�.

			unitRangeSystemObject->GetTransform()->scale = Vector2d(defaultEnemyIDRadius * 2, defaultEnemyIDRadius * 2);
			break;
		}
	}
	rangeSystemComponent->SetUnitComponent(unitComponent);


	return unitGameObject;
}
