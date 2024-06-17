#pragma once
#include "Projectile.h"
#include "ProjectilePool.h"

class CurveHomingProjectile : public Projectile
{
private:
	virtual void Update() override;

	virtual void OnContentsStop() override;

public:
	virtual void Init() override;
	friend ProjectilePool<CurveHomingProjectile>;
};

