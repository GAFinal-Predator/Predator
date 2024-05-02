#pragma once
#include "YunutyEngine.h"
#include "JsonUIData.h"
#include "UIImage.h"
#include "UIEnumID.h"

class LinearClippingTimer;
class UIButton;
class FloatFollower;
class SoundPlayingTimer;
class UIPriorityLayout;
class PopDownOnDisable;
class PopupOnEnable;
class TimePauseTimer;
class UIOffsetTransition;
class ColorTintTimer;
// 임포트된 UI 요소에 대한 정보를 잔뜩 저장하는 클래스
class UIElement : public Component, public ContentsObservee
{
public:
    virtual void Start() override;
    JsonUIData importedUIData;
    PopupOnEnable* scalePopUpTransition{ nullptr };
    PopDownOnDisable* scalePopDownTransition{ nullptr };
    UIOffsetTransition* enableTransition{ nullptr };
    UIOffsetTransition* disableTransition{ nullptr };
    ColorTintTimer* colorTintOnEnable{ nullptr };
    ColorTintTimer* colorTintOnDisable{ nullptr };
    TimePauseTimer* timePauseOnEnable{ nullptr };
    SoundPlayingTimer* soundOnClick{ nullptr };
    SoundPlayingTimer* soundOnHover{ nullptr };
    SoundPlayingTimer* soundOnEnable{ nullptr };
    SoundPlayingTimer* soundOnDisable{ nullptr };
    std::weak_ptr<graphics::UIImage> imageComponent{};
    graphics::UIText* textComponent{ nullptr };
    UIPriorityLayout* priorityLayout{ nullptr };
    UIPriorityLayout* parentPriorityLayout{ nullptr };
    UIButton* button{ nullptr };
    LinearClippingTimer* linearClippingTimerOnEnable{ nullptr };
    LinearClippingTimer* linearClippingTimerOnDisable{ nullptr };
    vector<UIElement*> children;
    void EnableElement();
    void DisableElement();
    void SetNumber(float number);
    // UI 요소에 영향을 줄 수 있는 실수 값을 조정합니다.
    FloatFollower* adjuster{ nullptr };

    virtual void PlayFunction() override;
    virtual void StopFunction() override;

private:
    // 복제된 UIElement의 하위 요소들에 대한 정보
    std::unordered_map<int, UIElement*> localUIsByIndex;
    std::unordered_map<UIEnumID, UIElement*> localUIsByEnumID;
    std::unordered_map<int, JsonUIData> localUIdatasByIndex;
    bool numberSetBefore = false;
    // 0~9까지의 숫자 이미지를 저장하는 배열
    array<yunuGI::ITexture*, 10>* digitFont{ };
    // 10^i 자리에 속하는 숫자의 이미지를 저장하는 배열
    std::vector<UIElement*> digits{ };
    friend class UIManager;
};
