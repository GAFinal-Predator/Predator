#include "InitialLoadingScreen.h"
#include "InWanderLand.h"
#include "VideoPlayer.h"
#include "WanderUtils.h"

void InitialLoadingScreen::Start()
{
    StartCoroutine(ShowLoadingScreen());
}
coroutine::Coroutine InitialLoadingScreen::ShowLoadingScreen()
{
    auto videoObj = Scene::getCurrentScene()->AddGameObject();
    auto uiImage = videoObj->AddComponent<graphics::UIImage>();
    auto videoPlayer = videoObj->AddComponent<graphics::VideoPlayer>();
    auto resol = graphics::Renderer::SingleInstance().GetResolution();
    uiImage->GetGI().SetLayer(1234567891);
    uiImage->GetGI().SetWidth(resol.x);
    uiImage->GetGI().SetHeight(resol.y);
    auto videoNailEngine = graphics::Renderer::SingleInstance().GetResourceManager()->GetVideoData(L"Texture/LoadingScreen/NailEngine.mp4");
    auto videoYunuty = graphics::Renderer::SingleInstance().GetResourceManager()->GetVideoData(L"Texture/LoadingScreen/YunutyEngine.mp4");
    auto videoJustLoading = graphics::Renderer::SingleInstance().GetResourceManager()->GetVideoData(L"Texture/LoadingScreen/LoadingResources.mp4");
    uiImage->GetGI().SetVideo(videoYunuty.lock()->GetName());
    videoPlayer->SetVideo(videoYunuty);
    videoPlayer->loop = false;

    while (!videoYunuty.lock()->IsDonePlaying())
    {
        if (Input::isKeyPushed(KeyCode::ESC))
        {
            break;
        }
        co_await std::suspend_always{};
    }
    co_await std::suspend_always{};
    videoPlayer->SetVideo(videoNailEngine);

    co_await std::suspend_always{};
    uiImage->GetGI().SetVideo(videoNailEngine.lock()->GetName());
    float t = 0;
    while (!videoNailEngine.lock()->IsDonePlaying())
    {
        t += Time::GetDeltaTimeUnscaled();
        if (Input::isKeyPushed(KeyCode::ESC))
        {
            break;
        }
        if (t > 4.6)
        {
            SoundSystem::PlaySoundfile("sounds/LoadingScreen/nailingSound.wav");
            t -= 10000;
        }
        co_await std::suspend_always{};
    }
    videoPlayer->SetVideo(videoJustLoading);
    videoPlayer->loop = true;
    co_await std::suspend_always{};

    uiImage->GetGI().SetVideo(videoJustLoading.lock()->GetName());
    while (wanderUtils::ResourceRecursiveLoader::IsLoadingResources())
    {
        co_await std::suspend_always{};
    }
    co_await std::suspend_always{};
#ifndef EDITOR
#ifndef GRAPHICS_TEST
    UIManager::Instance().GetUIElementByEnum(UIEnumID::TitleRoot)->EnableElement();
    UIManager::Instance().FadeOutRight(0);
    UIManager::Instance().FadeIn(2);
#endif
#endif 
    Scene::getCurrentScene()->DestroyGameObject(GetGameObject());
    Scene::getCurrentScene()->DestroyGameObject(videoObj);
    co_return;
}
