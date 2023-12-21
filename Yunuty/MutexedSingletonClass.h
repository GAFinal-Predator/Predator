#pragma once
#include "MutexedReference.h"

namespace yunutyEngine
{
    // �̱��� Ŭ���� �� ���� �����尡 ���ÿ� ��ü�� �����ϸ� �ȵǴ� Ŭ������ ����Ѵ�.
    template<typename T>
    class MutexedSingletonClass
    {
    public:
        static MutexedReference<T> SingleInstance();
    protected:
        virtual ~MutexedSingletonClass() = default;
    private:
        static std::mutex mtx;
        static T* instance;
    };
}
template<typename T>
yunutyEngine::MutexedReference<T> yunutyEngine::MutexedSingletonClass<T>::SingleInstance()
{
    if (instance == nullptr)
        instance = new T();
    return MutexedReference<T>(std::ref(instance), std::ref(mtx));
}
template<typename T>
T* yunutyEngine::MutexedSingletonClass<T>::instance = nullptr;
