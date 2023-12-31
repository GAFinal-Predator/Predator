#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <concepts>
#include "Object.h"
#include "Component.h"
#include "Rect.h"
#include "YunuGraphicsInterface.h"

// 카메라는 화면에 게임상에 존재하는 모든 그래픽 요소들을 출력하는 데에 쓰이는 객체입니다. 
// 카메라의 구현은 엔진의 동작환경,혹은 카메라의 특성에 따라 달라질 수 있기 때문에,
// 화면을 그려내기 위해 쓰이는 함수 Render는 추상 메소드로 정의됩니다.
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunuGI
{
	class IUnknown;
}
namespace yunutyEngine::graphics
{
	class Renderer;
	class Rendererable;
	class YunutyCycle;
	class YUNUTY_API EventHandler : public Component
	{
	public:
		static const set<EventHandler*>* GetEventHandlers(yunuGI::IRenderable*);
		static void CallEvents(yunuGI::IRenderable* renderable, function<void(EventHandler*)> todo);
		EventHandler();
		~EventHandler();
		void SetClickCallback(std::function<void()> clickCallback);
		void SetHoverCallback(std::function<void()> hoverCallback);
		//void SetTargetGraphic(Rendererable* target) { }
	protected:
	private:
		struct Impl;
		Impl* impl;
		friend Renderer;
	};
}
