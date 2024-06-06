#pragma once
#include "InWanderLand.h"
#include "YunutyEngine.h"
#include "FileSystem.h"

#include <string>
#include <future>
#include <thread>

using namespace application;
namespace wanderUtils
{
    class ResourceRecursiveLoader
    {
    public:
        static float coroutineDeltaTimeThreshold;
        static bool IsLoadingResources();
        static bool IsTimeToReturn(std::chrono::steady_clock::time_point& base)
        {
            if (auto now = std::chrono::high_resolution_clock::now(); std::chrono::duration_cast<std::chrono::milliseconds>(now - base).count() > 1000 * coroutineDeltaTimeThreshold)
            {
                base = now;
                return true;
            }
            return false;
        }
        // 사실상 얘는 로딩화면을 띄우기 위한 최소한의 리소스를 로드하는 데에만 쓰인다.
        static void Load(string rootFolder = "./", std::set<std::string> validExtensions = { ".jpg", ".bmp", ".tga", ".dds", ".cso",".png",".mp4" });
        struct boolGuard
        {
            boolGuard(bool& b) : b(b) {}
            ~boolGuard() { b = !b; }
            bool& b;
        };
        // 이 로드 함수는 파일을 부를떄 중간중간마다 리턴을 해준다.
        static coroutine::Coroutine LoadByCoroutine(string rootFolder = "./");
    private:
#ifdef EDITOR
        static std::unordered_map<string, float> timeTookToLoadResources;
#endif
        static bool isLoadingResources;
    };


    /// string 주소에 직접 접근하여 수정한 경우, Size 를 갱신하는 함수입니다.
    void UpdateStringSize(std::string& str)
    {
        int strSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0) - 1;
        str.resize(strSize);
    }

    /// wtring 주소에 직접 접근하여 수정한 경우, Size 를 갱신하는 함수입니다.
    void UpdateWStringSize(std::wstring& wstr)
    {
        int wstrSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr) - 1;
        wstr.resize(wstrSize);
    }
}
