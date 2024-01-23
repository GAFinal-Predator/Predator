/// 2023. 10. 04 김상준
/// SceneView 패널이 전체 공간에서 현재 확인하고 있는 부분을 한 눈에 파악할 수 있는
/// MiniMap 을 제공해주는 패널

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

namespace application
{
	namespace editor
	{
		class MiniMapPanel
			: public Panel, public Singleton<MiniMapPanel>
		{
			friend class Singleton<MiniMapPanel>;

		public:
			virtual ~MiniMapPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		private:
			MiniMapPanel();
		};
	}
}
