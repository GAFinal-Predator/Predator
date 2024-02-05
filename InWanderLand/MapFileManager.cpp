#include "InWanderLand.h"
#include "MapFileManager.h"
#include "Palette.h"
#include "Application.h"
#include "EditorEvents.h"
#include "EditorCommonEvents.h"

#include "Storable.h"
#include "InstanceManager.h"
#include "TemplateDataManager.h"
#include "Ornament_TemplateData.h"
#include "OrnamentData.h"
#include "UUIDManager.h"

#include <fstream>

namespace application
{
    namespace editor
    {
        MapFileManager::MapFileManager()
            : Singleton<MapFileManager>(), instanceManager(InstanceManager::GetSingletonInstance()), templateDataManager(TemplateDataManager::GetSingletonInstance())
            , currentMapPath()
        {

        }

        bool MapFileManager::LoadStaticOrnaments(const std::string& path)
        {
            std::ifstream loadFile{ path };

            if (loadFile.is_open())
            {
                json mapData;
                loadFile >> mapData;

                auto scene = yunutyEngine::Scene::getCurrentScene();
                const yunuGI::IResourceManager* resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

                auto objSize = mapData.size();

                std::string objName;
                std::string meshName;
                std::vector<std::string> materials;
                materials.reserve(4);
                std::vector<float> scale(3);
                std::vector<float> rotation(3);
                std::vector<float> location(3);

                for (int i = 0; i < objSize; i++)
                {
                    objName = mapData[i]["ObjectName"];
                    meshName = mapData[i]["MeshName"];
                    materials = mapData[i]["Material"];
                    scale[0] = mapData[i]["Scale"][0];
                    scale[1] = mapData[i]["Scale"][1];
                    scale[2] = mapData[i]["Scale"][2];
                    rotation[0] = mapData[i]["Rotation"][0];
                    rotation[1] = mapData[i]["Rotation"][1];
                    rotation[2] = mapData[i]["Rotation"][2];
                    location[0] = mapData[i]["Location"][0] / 100;
                    location[1] = mapData[i]["Location"][1] / 100;
                    location[2] = mapData[i]["Location"][2] / 100;

                    auto td = templateDataManager.CreateTemplateData<Ornament_TemplateData>(meshName);
                    if (td)
                    {
                        td->pod.meshKey = meshName;
                        td->pod.materialKey = materials;
                    }

                    auto odt = instanceManager.CreateInstance<OrnamentData>(meshName);

                    /// 좌표계 고려 변환
                    odt->pod.scale.x = scale[1];
                    odt->pod.scale.y = scale[2];
                    odt->pod.scale.z = scale[0];
                    auto quat = Quaternion(Vector3d(-rotation[1], -rotation[2], -rotation[0]));
                    odt->pod.rotation.x = quat.x;
                    odt->pod.rotation.y = quat.y;
                    odt->pod.rotation.z = quat.z;
                    odt->pod.rotation.w = quat.w;
                    odt->pod.position.x = location[1];
                    odt->pod.position.y = location[2];
                    odt->pod.position.z = location[0];
                    auto pi = odt->ApplyAsPaletteInstance();
                }

#ifdef EDITOR
                Application::DispatchEvent<LoadEvent>();
#endif
                return true;
            }
            else
            {
                return false;
            }
        }

        bool MapFileManager::LoadMapFile(const std::string& path)
        {
            std::ifstream loadFile{ path };

            if (loadFile.is_open())
            {
                json mapData;
                loadFile >> mapData;

                // 제대로 된 Map 파일이 아닐 경우 취소
                if (mapData.find("InWanderLand") == mapData.end())
                {
                    loadFile.close();
                    return false;
                }

                // Manager 초기화
                Clear();
#ifdef EDITOR
                // Palette 초기화
                palette::Palette::ResetPalettes();
#endif

                if (!instanceManager.PreDecoding(mapData) || !templateDataManager.PreDecoding(mapData))
                {
                    loadFile.close();
                    return false;
                }

                if (!instanceManager.PostDecoding(mapData) || !templateDataManager.PostDecoding(mapData))
                {
                    loadFile.close();
                    return false;
                }
                else
                {
                    loadFile.close();

#ifdef EDITOR
                    Application::DispatchEvent<LoadEvent>();
#endif

                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        bool MapFileManager::SaveMapFile(const std::string& path)
        {
            json mapData;
            // 저장을 더 진행하기 전에 각 데이터들의 내부 데이터를 pod 데이터로 이전하기 위해 실행하는 부분
            if (!instanceManager.PreSave() || !templateDataManager.PreSave())
            {
                return false;
            }
            // Pre
            if (!instanceManager.PreEncoding(mapData) || !templateDataManager.PreEncoding(mapData))
            {
                return false;
            }

            // Post
            if (!instanceManager.PostEncoding(mapData) || !templateDataManager.PostEncoding(mapData))
            {
                return false;
            }

            if (mapData.is_null())
            {
                // mapdData 가 만들어진 것이 없다면 취소
                return false;
            }

            mapData["InWanderLand"] = "Map";

            std::ofstream saveFile{ path };

            if (saveFile.is_open())
            {
                saveFile << mapData.dump(4);
                saveFile.close();

#ifdef EDITOR
                Application::DispatchEvent<SaveEvent>();
#endif

                return true;
            }
            else
            {
                return false;
            }
        }

        std::string MapFileManager::GetCurrentMapPath() const
        {
            return currentMapPath;
        }

        void MapFileManager::SetCurrentMapPath(std::string& path)
        {
            currentMapPath = path;
        }

        void MapFileManager::Clear()
        {
            instanceManager.Clear();
            templateDataManager.Clear();
            UUIDManager::GetSingletonInstance().Clear();
        }
    }
}
