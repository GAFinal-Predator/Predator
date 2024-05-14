#include "YunutyEngine.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace yunutyEngine;

Component::AddComponentDesc Component::addComponentDesc =
Component::AddComponentDesc
{
#if _DEBUG
    .addAvailable = false,
#endif
    .gameObject = nullptr,
    .guid = 0,
};
std::unordered_map<GUID, Component*, yutility::GenericHash<GUID>, yutility::GenericEqual<GUID>> Component::guidPtrMap;

Component::Component()
{
#if _DEBUG
    if (addComponentDesc.addAvailable != true)
    {
        _wassert(_CRT_WIDE("derived class of component class can only be created through the \"AddComponent\" function, which is the member function of gameobject class instance."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
    }
    addComponentDesc.addAvailable = false;
#endif
    gameObject = addComponentDesc.gameObject;
    if (addComponentDesc.guid == GUID_NULL)
        UuidCreate(&addComponentDesc.guid);
    guid = addComponentDesc.guid;
    guidPtrMap[guid] = this;

}
Component::~Component()
{
    guidPtrMap.erase(guid);

    gameObject->HandleComponentUpdateState(this);
}
yunutyEngine::GameObject* yunutyEngine::Component::GetGameObject()
{
    return gameObject;
}
const yunutyEngine::GameObject* yunutyEngine::Component::GetGameObject()const
{
    return gameObject;
}
Transform* yunutyEngine::Component::GetTransform()
{
    return gameObject->GetTransform();
}
const Transform* yunutyEngine::Component::GetTransform()const
{
    return gameObject->GetTransform();
}
bool yunutyEngine::Component::GetActive()
{
    return isActive;
}
GUID yunutyEngine::Component::GetGUID()
{
    return guid;
}
std::wstring yunutyEngine::Component::GetGUIDWStr()
{
    wchar_t guidStr[40];
    StringFromGUID2(guid, guidStr, sizeof(guidStr) / sizeof(wchar_t));
    return std::wstring(guidStr);
}
void yunutyEngine::Component::SetActive(bool active)
{
    if (isActive != active && GetGameObject()->GetActive())
    {
        isActive = active;
        gameObject->HandleComponentUpdateState(this);
        if (isActive)
            OnEnable();
        else
            OnDisable();
    }
    else
        isActive = active;
}
void yunutyEngine::Component::SetIsUpdating(bool isUpdating)
{
    if ((this->isUpdating != isUpdating) && GetGameObject()->GetActive())
    {
        this->isUpdating = isUpdating;
        gameObject->HandleComponentUpdateState(this);
    }
};
std::weak_ptr<yunutyEngine::Component::Coroutine> yunutyEngine::Component::StartCoroutine(Coroutine&& coroutine)
{
    auto coroutinePtr = std::make_shared<Coroutine>(std::move(coroutine));
    coroutines.insert(coroutinePtr);
    return coroutinePtr;
}
void yunutyEngine::Component::DeleteCoroutine(const std::weak_ptr<Coroutine>& coroutine)
{
    coroutines.erase(coroutine.lock());
}
