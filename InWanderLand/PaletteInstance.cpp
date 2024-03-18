#include "InWanderLand.h"
#include "PaletteInstance.h"
#include "DebugMeshes.h"
#include "Palette.h"

namespace application::editor::palette
{
    void PaletteInstance::Start()
    {
        InitPickingCollider();
    }
    void PaletteInstance::AdjustPickingCollider(const Vector3f& boundingMin, const Vector3f& boundingMax)
    {
        InitPickingCollider();
        pickingCollider->SetHalfExtent((boundingMax - boundingMin) / 2.0);
        pickingCollider->GetTransform()->SetLocalPosition((boundingMax + boundingMin) / 2.0);
        selectCircle->GetTransform()->SetLocalScale((boundingMax - boundingMin));
        selectCircle->GetTransform()->SetLocalPosition((boundingMax + boundingMin) / 2.0);
    }
    void PaletteInstance::OnHover()
    {
        isHovering = true;
        selectCircle->SetActive(true);
        selectCircle->GetGI().SetMaterial(0, GetColoredDebugMaterial({ 1,1,1 ,transparency }, false));
    }
    void PaletteInstance::OnHoverLeft()
    {
        isHovering = false;
        if (isSelected)
            OnSelected();
        else
        {
            selectCircle->SetActive(false);
        }
    }
    void PaletteInstance::OnSelected()
    {
        isSelected = true;
        selectCircle->SetActive(true);
        selectCircle->GetGI().SetMaterial(0, GetColoredDebugMaterial({ 0,1,0 ,transparency }, false));
    }
    void PaletteInstance::OnDeselected()
    {
        isSelected = false;
        if (isHovering)
            OnHover();
        else
        {
            selectCircle->SetActive(false);
        }
    }
    void PaletteInstance::EnablePickingCollider()
    {
        pickingCollider->SetActive(true);
    }
    void PaletteInstance::DisablePickingCollider()
    {
        pickingCollider->SetActive(false);
    }
    void PaletteInstance::InitPickingCollider()
    {
        if (selectCircle != nullptr || pickingCollider != nullptr)
        {
            assert(selectCircle && pickingCollider);
            return;
        }
        selectCircle = AttachDebugMesh(GetGameObject()->AddGameObject(), DebugMeshType::Cube, yunuGI::Color{1, 1, 1, transparency}, false);
        //selectCircle->GetTransform()->SetLocalRotation(Quaternion({ 90,0,0 }));
        auto colliderObject = GetGameObject()->AddGameObject();
        pickingCollider = colliderObject->AddComponent<yunutyEngine::physics::BoxCollider>();
        rigidBody = colliderObject->AddComponent<yunutyEngine::physics::RigidBody>();
        pickingCollider->SetHalfExtent({ 0.5,0.5,0.5 });
        rigidBody->SetAsKinematic(true);
        selectCircle->GetTransform()->SetLocalScale(Vector3d::one * selectCircleRadius);
        selectCircle->SetActive(false);
    }
    void PaletteInstance::ResetInstances()
    {
    }
    IEditableData* PaletteInstance::GetEditableData()
    {
        return editableData;
    }
    void PaletteInstance::SetEditableData(IEditableData* data)
    {
        editableData = data;
    }
    void PaletteInstance::ApplyInstances()
    {
    }
}
