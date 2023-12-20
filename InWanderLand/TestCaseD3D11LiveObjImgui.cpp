#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"
#include "RTSCam.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "ShakyCam.h"
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "YunutyEngine.h"
#include <codecvt>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void TestCaseD3D11LiveObjimguiInit()
{
}

namespace InWanderLand
{
    TEST_CLASS(InWanderLand)
    {
    public:
        TEST_METHOD(TestCaseD3D11LiveObjimgui)
        {
            try
            {
                application::Application& client = application::Application::CreateApplication(0, 0);
                application::Contents::ContentsLayer::AssignTestInitializer(TestCaseD3D11LiveObjimguiInit);
                client.Initialize();
                client.Run();
                client.Finalize();
            }
            catch (const std::exception& e)
            {
                Assert::Fail(yunutyEngine::yutility::GetWString(e.what()).c_str());
            }
            //Assert::IsTrue();
        }
    };
}
#endif