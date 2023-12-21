#pragma once
#include <mutex>

namespace yunutyEngine
{
    // mutex�� �̿��� �ϳ��� ��ü ���۷����� ���� ���� ��������� ������ �����ϴ� Ŭ����
    // �� ��ü�� RAII�� ����ϹǷ�, ���۷����� ����� �� ������ �� ���ҽ��� ���� �������¸� �����ϱ� ���ؼ��� �� ��ü�� �����Ǿ�� �Ѵ�. 
    template <typename T>
    class MutexedReference {
    public:
        MutexedReference(T& ref, std::mutex& mtx) : data(ref), mutex(mtx) {
            mutex.lock();
        }

        ~MutexedReference() {
            mutex.unlock();
        }

        T& operator*() {
            return data;
        }

    private:
        T& data;
        std::mutex& mutex;
    };
}
