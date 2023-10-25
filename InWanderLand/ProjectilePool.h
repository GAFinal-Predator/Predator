#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Vector3.h"
#include "unordered_set"
#include "Projectile.h"
#include "Bullet.h"
//#include "ProjectileManager.h"
#include <concepts>

// static void CreateProjectile() �� �ִ� Ŭ������ ���ø� �Ķ���ͷ� ���� ���� �������ִ� concept�Դϴ�.
template <class T>
concept ProjectileTypeConcept = requires(T t) {
	{ T::CreateProjectile() } -> std::same_as<void>;
};

/// <summary>
/// Projectile���� �����صδ� ProjectilePool Ŭ�����Դϴ�.
/// ������ ������ concept ���� ���� static void CreateProjectile()�� �ִ� 
/// Ŭ�������� ���ø� �Ķ���ͷ� �ް� �˴ϴ�.
/// �� Ŭ�������� Projectile�� Pool���� ������, �ٽ� ȸ���ϴ� ����� �ֽ��ϴ�.
/// �׸��� Pool�� ProjectileType���� �ϳ��� �ֱ� ������ Singleton���� �����˴ϴ�.
/// </summary>

template <ProjectileTypeConcept ProjectileType>
class ProjectilePool : public yunutyEngine::Component
{
public:
	ProjectilePool();
	static ProjectilePool<ProjectileType>* GetInstance() { return instance; }
	Projectile* SummonProjectile(yunutyEngine::Vector3d position, yunutyEngine::Vector3d speed, float damage, bool isPlayerProjectile);
	void RetrieveProjectile(Projectile* projectile);

private:
	static ProjectilePool<ProjectileType>* instance;
	std::unordered_set<Projectile*> projectiles;
	//friend class ProjectileManager;
};


template<ProjectileTypeConcept ProjectileType>
ProjectilePool<ProjectileType>* ProjectilePool<ProjectileType>::instance = nullptr;

template <ProjectileTypeConcept ProjectileType>
ProjectilePool<ProjectileType>::ProjectilePool()
{
	instance = this;
	//ProjectileManager::GetInstance()->projectilePoolList.insert(instance); 
	instance->projectiles.clear();
}

template <ProjectileTypeConcept ProjectileType>
Projectile* ProjectilePool<ProjectileType>::SummonProjectile(yunutyEngine::Vector3d position, yunutyEngine::Vector3d speed, float damage, bool isPlayerProjectile)
{
	if (instance == nullptr || instance->projectiles.empty())
	{	
		for (int i = 0; i < 10; ++i)
			ProjectileType::CreateProjectile();
	}
	auto it = instance->projectiles.begin();
	auto pProj = *it;
	pProj->GetTransform()->SetWorldPosition(position);
	pProj->speed = speed;
	pProj->damage = damage;
	pProj->isPlayerProjectile = isPlayerProjectile;
	pProj->GetGameObject()->SetSelfActive(true);
	instance->projectiles.erase(it);
	return pProj;
}

template <ProjectileTypeConcept ProjectileType>
void ProjectilePool<ProjectileType>::RetrieveProjectile(Projectile* projectile)
{
	instance->projectiles.insert(projectile);
	projectile->GetGameObject()->SetSelfActive(false);
}