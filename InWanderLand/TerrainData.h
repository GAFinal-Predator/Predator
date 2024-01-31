/// 2023. 11. 23 김상준
/// IEditableData 의 구체화된 클래스
/// 지형

#pragma once

#include "IEditableData.h"
#include "Terrain_TemplateData.h"

#include <memory>
#include <string>

namespace application
{
    namespace editor
    {
        class TemplateDataManager;
    }
}

namespace application
{
    namespace editor
    {
        class TerrainData;
        struct POD_Terrain
        {
            Terrain_TemplateData* templateData = nullptr;
            std::vector<std::pair<int, int>> coordinates;
            std::vector<float> heights;

            TO_JSON(POD_Terrain)
                FROM_JSON(POD_Terrain)
        };

        class TerrainData
            : public IEditableData
        {
            friend class InstanceManager;

        public:
            struct Node
            {
                float height;
                GameObject* debugObject;
            };
            ~TerrainData();
            // 지형 인스턴스 정보는 맵에 단 하나만 존재합니다.
            static TerrainData& Instance();
            virtual bool EnterDataFromTemplate() override;
            virtual ITemplateData* GetTemplateData() override;
            virtual bool SetTemplateData(const std::string& dataName) override;
            virtual IEditableData* Clone() const override;
            virtual palette::PaletteInstance* ApplyAsPaletteInstance()override;

            // 팔레트에서 이전된 기능들
            void ApplyAsPlaytimeObject();
            void AddNode(const Vector2i& nodeKey, Node&& nodeInfo);
            void EraseNode(const Vector2i& nodeKey);
            void ClearNodes();
            Vector3d GetNodePosition(const Vector2i& nodeKey);
            Vector2i WorldToNodeSpace(const Vector3d& worldPos);
            GameObject* CreateNodeDebuggingMesh(const Vector2i& nodeKey);

            POD_Terrain pod;

            static constexpr double nodeDistance = 0.5;
        protected:
            virtual bool PreSaveCallback()  override;
            virtual bool PreEncoding(json& data) const override;
            virtual bool PostEncoding(json& data) const override;
            virtual bool PreDecoding(const json& data) override;
            virtual bool PostDecoding(const json& data) override;

        private:
            unordered_map<Vector2i, Node> nodes;
            static TerrainData* soleTerrainData;
            static Terrain_TemplateData* soleTerrainTemplateData;
            static TemplateDataManager& templateDataManager;

            TerrainData();
            TerrainData(const std::string& name);
            TerrainData(const TerrainData& prototype);
            TerrainData& operator=(const TerrainData& prototype);
        };
    }
}
