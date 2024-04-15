#include "InWanderLand.h"
#include "Identifiable.h"

#include "UUIDManager.h"

namespace application
{
	std::string UUID_To_String(UUID uuid)
	{
		char* id_char;
		if (UuidToStringA(&uuid, (RPC_CSTR*)&id_char) != RPC_S_OK)
		{
			// 오류 시 빈 string 반환
			return std::string();
		}

		return std::string(id_char);
	}

	UUID String_To_UUID(std::string str)
	{
		UUID uuid;
		if (UuidFromStringA((RPC_CSTR)str.c_str(), &uuid) != RPC_S_OK)
		{
			// 오류 시 빈 UUID 반환
			return UUID();
		}

		return uuid;
	}

	Identifiable::~Identifiable()
	{
		UUIDManager::GetSingletonInstance().EraseUUID(id);
	}

	Identifiable::Identifiable()
	{
		UuidCreate(&id);
		UUIDManager::GetSingletonInstance().RegisterUUIDWithPointer(id, this);
	}

	Identifiable::Identifiable(const UUID& id)
		: id(id)
	{
		UUIDManager::GetSingletonInstance().RegisterUUIDWithPointer(id, this);
	}

	UUID Identifiable::GetUUID() const
	{
		return id;
	}

	void Identifiable::SetUUID(const UUID& id)
	{
		UUIDManager::GetSingletonInstance().EraseUUID(this->id);
		this->id = id;
		UUIDManager::GetSingletonInstance().RegisterUUIDWithPointer(id, this);
	}
}
