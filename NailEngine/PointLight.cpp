#include "PointLight.h"

PointLight::PointLight()
{
	this->lightInfo.lightType = static_cast<unsigned int>(LightType::Point);
	this->lightInfo.range = 1.f;

	this->meshName = L"Sphere";
	this->materialName = L"Deferred_PointLight";
}

void PointLight::SetLightDiffuseColor(DirectX::SimpleMath::Vector4& color)
{
	this->lightInfo.color.diffuse = color;
	this->lightInfo.color.specular = color;
}

void PointLight::SetLightPosition(DirectX::SimpleMath::Vector4& position)
{
	this->lightInfo.position = position;
}

void PointLight::SetRange(float range)
{
	this->lightInfo.range = range;
}

void PointLight::SetWorldTM(const DirectX::SimpleMath::Matrix& wtm)
{
	this->wtm = wtm;
}

DirectX::SimpleMath::Matrix& PointLight::GetWorldTM()
{
	this->wtm._11 = this->lightInfo.range * 2;
	this->wtm._22 = this->lightInfo.range * 2;
	this->wtm._33 = this->lightInfo.range * 2;
	return this->wtm;
}
