#pragma once
#include <concepts>
#include "YunutyEngine.h"

namespace yunutyEngine
{
    // ȣ��Ǹ� �ٷ� �̱��� Ŭ������ �ڽ� Ŭ����Ÿ���� ���ø� �Ķ���ͷ� �޴� ���ø� Ŭ������ �����Ѵ�.
    template<typename T> requires std::derived_from<T, yunutyEngine::Component>
    class SingletonComponent
    {
    public:
        static T& Instance();
        static bool isAvailable() { return yunutyEngine::Scene::getCurrentScene(); }
    protected:
        virtual ~SingletonComponent() = default;
    private:
        static T* instance;
    };
}
template<typename T>
T& yunutyEngine::SingletonComponent<T>::Instance()
{
    if (instance == nullptr)
    {
        instance = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<T>();
    }
    return *instance;
}
template<typename T>
T* yunutyEngine::SingletonComponent<T>::instance = nullptr;
