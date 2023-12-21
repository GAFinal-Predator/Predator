/// 2023. 11. 01 �����
/// Unit, Terrain �� ������� ���� ���� �� �ɼ��� �����ϴ� �ȷ�Ʈ �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class PalettePanel
			: public Panel, public Singleton<PalettePanel>
		{
		public:
			PalettePanel();
			virtual ~PalettePanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
