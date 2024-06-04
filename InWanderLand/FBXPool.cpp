#include "FBXPool.h"
#include "InWanderLand.h"
#include <DirectXMath.h>

// Borrow 함수는 먼저 해당 fbx 이름에 맞는 풀이 존재하는지 확인하고, 없으면 새로 만든다.
std::weak_ptr<ManagedFBX> FBXPool::Borrow(const std::string& fbxName)
{
    if (!poolsByMeshName.contains(fbxName))
    {
        auto pool = std::make_shared<PoolByMesh>();
        pool->fbxName = fbxName;
        poolsByMeshName[fbxName] = pool;
    }
    return poolsByMeshName[fbxName]->Borrow();
}

void FBXPool::Return(std::weak_ptr<ManagedFBX> fbx)
{
    poolsByFBX.at(fbx.lock().get()).lock()->Return(fbx);
}

void FBXPool::PoolByMesh::ObjectInitializer(std::weak_ptr<ManagedFBX> projectile)
{
    auto gameObj = Scene::getCurrentScene()->AddGameObjectFromFBX(fbxName);
    FBXPool::SingleInstance().poolsByFBX[projectile.lock().get()] = FBXPool::SingleInstance().poolsByMeshName.at(fbxName);
}

void ManagedFBX::OnContentsStop()
{
    FBXPool::SingleInstance().Return(GetWeakPtr<ManagedFBX>());
}
