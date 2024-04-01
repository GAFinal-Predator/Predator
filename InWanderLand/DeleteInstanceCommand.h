/// 2024. 03. 29 김상준
/// Instance 를 삭제할 때의 작업을 위한 Command
/// 연계된 TransformEditCommand 를 삭제할 수 있도록 함

#pragma once

#include "Command.h"
#include "IEditableData.h"

#include <vector>

namespace application
{
	namespace editor
	{
		/// 추후 UndoableCommand 로 개선해야 함
		class DeleteInstanceCommand
			: public Command
		{
		public:
			COMMAND_SETTING(CommandType::Delete)

			virtual ~DeleteInstanceCommand() = default;
			DeleteInstanceCommand(const std::vector<IEditableData*>& list);

			virtual void Execute() override;

		private:
			std::vector<IEditableData*> list;
		};
	}
}

