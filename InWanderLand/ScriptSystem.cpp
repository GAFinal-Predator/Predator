#include "ScriptSystem.h"

#include "Script.h"
#include "YunutyEngine.h"

namespace application
{
	std::vector<std::pair<TriggerType, std::string>> ScriptSystem::triggerList = std::vector<std::pair<TriggerType, std::string>>();
	std::vector<std::pair<ConditionType, std::string>> ScriptSystem::conditionList = std::vector<std::pair<ConditionType, std::string>>();
	std::vector<std::pair<ActionType, std::string>> ScriptSystem::actionList = std::vector<std::pair<ActionType, std::string>>();

	void ScriptSystem::SingletonInitializer()
	{
		/// Trigger
		triggerList.push_back({ TriggerType::GameStart,"GameStart" });
		triggerList.push_back({ TriggerType::EnterRegion, "EnterRegion" });
		triggerList.push_back({ TriggerType::LeaveRegion, "LeaveRegion" });
		triggerList.push_back({ TriggerType::RepeatPeriodically, "RepeatPeriodically" });
		triggerList.push_back({ TriggerType::RepeatPeriodicallyRealTime, "RepeatPeriodicallyRealTime" });
		triggerList.push_back({ TriggerType::UnitAppear, "UnitAppear" });
		triggerList.push_back({ TriggerType::UnitDie, "UnitDie" });

		/// Condition
		conditionList.push_back({ ConditionType::CinematicModeOn, "CinematicModeOn" });
		conditionList.push_back({ ConditionType::CinematicModeOff, "CinematicModeOff" });
		conditionList.push_back({ ConditionType::TutorialModeOn, "TutorialModeOn" });
		conditionList.push_back({ ConditionType::TutorialModeOff, "TutorialModeOff" });

		/// Action
		actionList.push_back({ ActionType::WaitPreviousActionEnd, "WaitPreviousActionEnd" });
		actionList.push_back({ ActionType::WaitForSeconds, "WaitForSeconds" });
		actionList.push_back({ ActionType::WaitForRealSeconds, "WaitForRealSeconds" });
		actionList.push_back({ ActionType::CinematicModeChange, "CinematicModeChange" });
		actionList.push_back({ ActionType::CinematicFadeIn, "CinematicFadeIn" });
		actionList.push_back({ ActionType::CinematicFadeOut, "CinematicFadeOut" });
		actionList.push_back({ ActionType::CameraChangeView, "CameraChangeView" });
		actionList.push_back({ ActionType::CameraRevert, "CameraRevert" });
		actionList.push_back({ ActionType::CameraSaveView, "CameraSaveView" });
		actionList.push_back({ ActionType::CameraLoadView, "CameraLoadView" });
		actionList.push_back({ ActionType::TutorialModeChange, "TutorialModeChange" });
		actionList.push_back({ ActionType::PlayManualDialogue, "PlayManualDialogue" });
		actionList.push_back({ ActionType::PlayTimedDialogue, "PlayTimedDialogue" });
		actionList.push_back({ ActionType::UnitMove, "UnitMove" });
	}

	Script* ScriptSystem::CreateScript()
	{
		auto sptr = Scene::getCurrentScene()->AddGameObject()->AddComponent<Script>();
		scriptList[sptr->GetUUID()] = sptr;
		scriptContainer.insert(sptr);
		return sptr;
	}

	bool ScriptSystem::EraseScript(Script* script)
	{
		auto itr = scriptList.find(script->GetUUID());
		if (itr == scriptList.end())
		{
			return false;
		}

		Scene::getCurrentScene()->DestroyGameObject(script->GetGameObject());

		scriptList.erase(itr);
		scriptContainer.erase(script);

		return true;
	}

	void ScriptSystem::PreMapLoad()
	{
		Action_CameraSaveView::Clear();

		Clear();
	}

	void ScriptSystem::OnGameStart()
	{
		Action_CameraSaveView::Clear();

		for (auto& each : scriptList)
		{
			each.second->OnGameStart();
		}
	}

	void ScriptSystem::OnGameStop()
	{
		Action_CameraSaveView::Clear();

		for (auto& each : scriptList)
		{
			each.second->OnGameStop();
		}
	}

	std::unordered_set<Script*>& ScriptSystem::GetScriptList()
	{
		return scriptContainer;
	}

	void ScriptSystem::Clear()
	{
		for (auto& each : scriptList)
		{
			Scene::getCurrentScene()->DestroyGameObject(each.second->GetGameObject());
		}
		scriptList.clear();
		scriptContainer.clear();
	}

	bool ScriptSystem::PreEncoding(json& data) const
	{
		std::string uuidStr;
		for (auto& [uuid, ptr] : scriptList)
		{
			uuidStr = UUID_To_String(uuid);

			if (!ptr->PreEncoding(data["ScriptList"][uuidStr]))
			{
				return false;
			}
		}

		return true;
	}

	bool ScriptSystem::PostEncoding(json& data) const
	{
		std::string uuidStr;
		for (auto& [uuid, ptr] : scriptList)
		{
			uuidStr = UUID_To_String(uuid);

			auto itr = data["ScriptList"].find(uuidStr);
			if (itr == data["ScriptList"].end())
			{
				return false;
			}

			if (!ptr->PostEncoding(itr.value()))
			{
				return false;
			}
		}

		return true;
	}

	bool ScriptSystem::PreDecoding(const json& data)
	{
		if (!data.contains("ScriptList"))
			return true;
		UUID uuid;
		for (auto& [uuidStr, scriptData] : data["ScriptList"].items())
		{
			uuid = String_To_UUID(uuidStr);

			auto script = CreateScript();

			if (script == nullptr)
			{
				Clear();
				return false;
			}

			scriptList.erase(script->GetUUID());

			// UUID
			script->SetUUID(uuid);
			scriptList[uuid] = script;

			if (!script->PreDecoding(scriptData))
			{
				Clear();
				return false;
			}
		}

		return true;
	}

	bool ScriptSystem::PostDecoding(const json& data)
	{
		if (!data.contains("ScriptList"))
			return true;
		UUID uuid;
		for (auto& [uuidStr, scriptData] : data["ScriptList"].items())
		{
			uuid = String_To_UUID(uuidStr);

			for (auto& each : scriptList)
			{
				if (each.first == uuid)
				{
					if (!each.second->PostDecoding(scriptData))
					{
						Clear();
						return false;
					}

					break;
				}
			}
		}

		return true;
	}
}
