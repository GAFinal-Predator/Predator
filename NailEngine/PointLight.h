#pragma once
#include "ILight.h"

#include <DirectXMath.h>
#include "SimpleMath.h"
using namespace DirectX::PackedVector;

class PointLight : public ILight
{
public:
	PointLight();

public:
	virtual void SetLightDiffuseColor(DirectX::SimpleMath::Vector4& color) override;
	virtual void SetLightPosition(DirectX::SimpleMath::Vector4& position) override;

public:
	void SetRange(float range);
	void SetWorldTM(const DirectX::SimpleMath::Matrix& wtm);
	DirectX::SimpleMath::Matrix& GetWorldTM();

private:
	DirectX::SimpleMath::Matrix wtm;

};

