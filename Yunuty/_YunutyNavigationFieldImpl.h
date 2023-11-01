#pragma once
#include "YunutyNavigationField.h"
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

namespace yunutyEngine
{
    // Impl�� ���� �����͸� �׾Ƶδ� ������ ����.
    class NavigationField::Impl
    {
    private:
        Impl(NavigationField* navFieldComponent) :navFieldComponent(navFieldComponent)
        {
            navQuery = dtAllocNavMeshQuery();
            crowd = dtAllocCrowd();
            context = std::make_unique<rcContext>(rcContext());
        }
        virtual ~Impl()
        {
            dtFreeCrowd(crowd);
            dtFreeNavMeshQuery(navQuery);
        }
        friend NavigationField;
    public:
        NavigationField* navFieldComponent;

        std::unique_ptr<rcContext> context;
        rcPolyMesh* polyMesh;
        rcConfig config;
        rcPolyMeshDetail* polyMeshDetail;
        class dtNavMesh* navMesh;
        class dtNavMeshQuery* navQuery;
        class dtCrowd* crowd;
    };
}
