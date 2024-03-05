#include "UIButton.h"
#include "UIManager.h"
#include "RTSCam.h"

void UIButton::SetIdleImage(yunuGI::ITexture* p_IdleImage)
{
	m_IdleImage = p_IdleImage;
	m_ImageComponent->GetGI().SetImage(m_IdleImage);
}

void UIButton::SetOnMouseImage(yunuGI::ITexture* p_OnMouseImage)
{
	m_MouseOnImage = p_OnMouseImage;
}

void UIButton::SetClickedImage(yunuGI::ITexture* p_ClickedImage)
{
	m_ClickedImage = p_ClickedImage;
}

void UIButton::SetButtonClickFunction(std::function<void()> p_func)
{
	m_mouseLiftedEventFunction = p_func;
}

void UIButton::SetLayer(int p_layerNum)
{
	m_layer = p_layerNum;
	m_ImageComponent->GetGI().SetLayer(p_layerNum);
}

int UIButton::GetLayer() const
{
	return m_layer;
}

void UIButton::SetImageComponent(yunutyEngine::graphics::UIImage* p_ImageComponent)
{
	m_ImageComponent = p_ImageComponent;
}

void UIButton::Start()
{
	assert(m_IdleImage != nullptr);

	m_ImageComponent->GetGI().SetImage(m_IdleImage);
	//m_CurrentImage = m_IdleImage;

	/// Width와 Height은 변경되지 않는다는 것을 전제로...
	m_Width = m_IdleImage->GetWidth();
	m_Height = m_IdleImage->GetHeight();

	Vector2d leftTopPos = GetTransform()->GetWorldPosition();
	m_ImageCenterPostion = Vector2d(leftTopPos.x + m_Width / 2, leftTopPos.y + m_Height / 2);

	m_onMouseFunction = [=]()
	{
		if (m_MouseOnImage != nullptr)
		{
			m_ImageComponent->GetGI().SetImage(m_MouseOnImage);
		}
		if (m_OnMouseEventFunction != nullptr)
		{
			m_OnMouseEventFunction();
		}
	};

	m_mousePushedFunction = [=]()
	{
		if (m_ClickedImage != nullptr)
		{
			m_ImageComponent->GetGI().SetImage(m_ClickedImage);
		}
	};

	m_mouseLiftedFunction = [=]()
	{
		if (m_MouseOnImage != nullptr)
		{
			m_ImageComponent->GetGI().SetImage(m_MouseOnImage);
		}
		if (m_mouseLiftedEventFunction != nullptr)
		{
			m_mouseLiftedEventFunction();
		}
	};



}

void UIButton::Update()
{
	auto mousePos = Input::getMouseScreenPosition();
	bool isMouseJustEntered = false;

	if (mousePos.x <= m_ImageCenterPostion.x + m_Width / 2 && mousePos.x >= m_ImageCenterPostion.x - m_Width / 2 &&
		mousePos.y <= m_ImageCenterPostion.y + m_Height / 2 && mousePos.y >= m_ImageCenterPostion.y - m_Height / 2)
	{
		isMouseJustEntered = true;
	}

	if (isMouseJustEntered && !isMouseNowOnButton)
	{
		isMouseNowOnButton = true;
		isMouseJustEntered = false;

		UIManager::SingleInstance().ReportButtonOnMouse(this);
		//isButtonOnMouseState = false;
	}
	else if (!isMouseJustEntered && isMouseNowOnButton)
	{
		isMouseNowOnButton = false;
		UIManager::SingleInstance().ReportMouseExitButton(this);
		m_ImageComponent->GetGI().SetImage(m_IdleImage);
	}

}
