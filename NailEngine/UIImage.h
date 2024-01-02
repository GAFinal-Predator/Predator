#pragma once
#include "IRenderable.h"

class UIImage : public IRenderable
{
public:
	void SetTexture(yunuGI::ITexture* texture)
	{
		this->texture = texture;
	}

	yunuGI::ITexture* GetTexture()
	{
		return this->texture;
	}

	bool operator<(const UIImage& other) const
	{
		return layer < other.layer;
	}

public:
	DirectX::SimpleMath::Vector2 pos;
	int layer = 0;

private:
	yunuGI::ITexture* texture;
};

struct CompareSmartPtr {
	bool operator()(const std::shared_ptr<IRenderable>& lhs, const std::shared_ptr<IRenderable>& rhs) const {
		const auto& leftImage = std::static_pointer_cast<UIImage>(lhs);
		const auto& rightImage = std::static_pointer_cast<UIImage>(rhs);

		if (leftImage->layer != rightImage->layer) {
			return leftImage->layer < rightImage->layer; // �ٸ� layer�� ��쿡�� ��
		}

		// ���߿� �̹����� layer�� �̻��ϴٸ� �� �ڵ带 �� �� �ּҷ� ���ϱ⿡ � ������ �Ͼ �� Ȯ���� �� ����

		return leftImage.get() < rightImage.get();
	}
};