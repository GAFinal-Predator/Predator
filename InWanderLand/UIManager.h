#pragma once
#include "YunutyEngine.h"
#include "UIButton.h"
#include "Storable.h"
#include "UIEnumID.h"
#include "JsonUIData.h"
/// <summary>
/// UIButton 객체들을 관리하는 컴포넌트.
/// 역할
/// 1. 모든 UIButton 객체들을 저장 및 관리
/// 2. button 멤버 UIImage의 Layer를 비교하여 클릭 함수 재정의
/// 3. 최소 하나의 button에 마우스가 올라가 있는가? 를 항상 판별.
/// 4. 유니티 에디터에서 만든 UI 레이아웃을 불러와서 생성
/// </summary>

class UIElement;
class UIManager : public Component, public SingletonComponent<UIManager>
{
private:
    // JsonUIData만으로 UI를 생성합니다.
    void ImportDefaultAction(const JsonUIData& uiData, UIElement* element);
    void ImportDefaultAction_Post(const JsonUIData& uiData, UIElement* element);
    // 특별한 로직이 적용되어야 하는 경우 참, 그렇지 않으면 거짓을 반환합니다.
    bool ImportDealWithSpecialCases(const JsonUIData& uiData, UIElement* element);
    // 아래 두 함수들을 응용해 UI들이 다 생성되고 난 후 추가적인 작업을 수행합니다.
    bool ImportDealWithSpecialCases_Post(const JsonUIData& uiData, UIElement* element);
    int uiImportingPriority{ 0 };
    struct ButtonCompare
    {
        bool operator()(const UIButton* lhs, const UIButton* rhs) const
        {
            return lhs->GetLayer() > rhs->GetLayer();
        }
    };
    std::set<UIButton*, ButtonCompare> m_selectedButtons;

    std::function<void(Vector3d pos)> m_beforeUIEnterFunction;

    int m_currentHighestLayer = 0;
    UIButton* m_highestPriorityButton;

    bool isButtonActiviated = false;
    std::unordered_map<int, UIElement*> uisByIndex;
    std::unordered_map<UIEnumID, UIElement*> uisByEnumID;
    std::unordered_map<int, JsonUIData> uidatasByIndex;
    std::unordered_map<UIElement*, array<yunuGI::ITexture*, 10>> digitFonts;

public:

    void Clear();
    void FadeOutRight(float duration = 1);
    void FadeOutLeft(float duration = 1);
    void FadeOutTop(float duration = 1);
    void FadeOutBottom(float duration = 1);
    void FadeIn(float duration = 1);
    void SetIngameUIVisible(bool visible);
    void ReportButtonOnMouse(UIButton* p_btn);
    void ReportMouseExitButton(UIButton* p_btn);
    // 버튼 중 가장 높은 우선순위를 가진 버튼으로 highestpriorityButton을 재설정합니다.
    // 현재의 highestprirorityButton은 Exit 이벤트 함수가 호출되며, 새로운 highestpriorityButton은 Enter 이벤트 함수가 호출됩니다.
    // 만약 현재의 highestPirorityButton이 여전히 가장 높은 우선순위를 가지고 있다면, 아무 일도 벌어지지 않습니다.
    void UpdateHighestPriorityButton();

    bool IsMouseOnButton();
    UIElement* GetUIElementByEnum(UIEnumID uiEnumID);
    void ImportUI(const char* path);

    virtual void Update() override;
};

