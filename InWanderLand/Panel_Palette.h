/// 2023. 11. 01 �����
/// Unit, Terrain �� ������� ���� ���� �� �ɼ��� �����ϴ� �ȷ�Ʈ �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class PalettePanel
			: public Panel
		{
		public:
			static PalettePanel& GetInstance();

			virtual ~PalettePanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<PalettePanel> instance;

			PalettePanel();
			PalettePanel(const PalettePanel& copy) = delete;
			PalettePanel& operator=(const PalettePanel& copy) = delete;
		};
	}
}
