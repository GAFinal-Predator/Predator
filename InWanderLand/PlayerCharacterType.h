#pragma once
#include "PodStructs.h"

struct PlayerCharacterType
{
    enum Enum {
        None,
        Robin,
        Ursula,
        Hansel,
        End,
    };
};

template<>
const std::unordered_map<int, std::string>& application::POD_Enum<PlayerCharacterType::Enum>::GetEnumNameMap()
{
    static std::unordered_map<int, std::string> enumNameMap
    {
        { (int)PlayerCharacterType::None, "None" },
        { (int)PlayerCharacterType::Robin, "Robin" },
        { (int)PlayerCharacterType::Ursula, "Ursula" },
        { (int)PlayerCharacterType::Hansel, "Hansel" },
        { (int)PlayerCharacterType::End, "End"}
    };
    return enumNameMap;
}
