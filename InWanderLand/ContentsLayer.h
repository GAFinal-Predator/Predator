/// 2023. 10. 11 김상준
/// Contents 제작을 위한 레이어

#pragma once

#include "Layer.h"
#include <functional>

namespace application
{
    namespace Contents
    {
        class ContentsLayer
            : public Layer
        {
        private:
            static std::function<void()> testInitializer;
        public:
            virtual void Initialize() override;
            //virtual void EventProgress() override;
            virtual void Update(float ts) override;
            virtual void GUIProgress() override;
            virtual void Finalize() override;
            // 테스트 코드에서 임의로 ContentsLayer의 Initialize 코드를 오버라이드 하고 싶을때 쓰이는 함수
            // 이 함수를 호출하면 YunutyCycle에서 발생한 예외를 잡아서 테스트 스레드에서 예외를 던집니다.
#ifdef GEN_TESTS
            static void AssignTestInitializer(std::function<void()> testInitializer);
#endif
        };
    }
}
