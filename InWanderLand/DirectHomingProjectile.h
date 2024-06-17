#pragma once
#include "Projectile.h"
#include "ProjectilePool.h"

class DirectHomingProjectile : public Projectile
{
private:
	virtual void Update() override;

public:
	virtual void Init() override;
	friend ProjectilePool<DirectHomingProjectile>;
};
