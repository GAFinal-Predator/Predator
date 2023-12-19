/// 2023. 11. 20 �����
/// �����Ϳ��� ����� �ν��Ͻ����� �����ϱ� ���Ͽ� ������ �ĺ��ڸ� ����ϱ� ���� ����ü
/// �ش� ����ü�� MS ���� �������ִ� �⺻ ����ü�� �����

#pragma once

#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

// We will be able to use the 'UUID' structure.

#include <xhash>

namespace std 
{
    template <>
    class hash<UUID>
    {
    public:
        size_t operator()(const UUID& uuid) const 
        {
            return *reinterpret_cast<size_t*>(const_cast<UUID*>(&uuid)->Data4);
        }
    };
};
