#include "YunutyEngine.h"
#include "Animator.h"
#include "_YunuGIObjects.h"
using namespace yunutyEngine::graphics;

yunutyEngine::graphics::Animator::Animator()
	: Renderable<yunuGI::IAnimator>(_YunuGIObjects::SingleInstance().factory->CreateAnimator({}))
{

}

void Animator::Update()
{
	auto& gi = this->GetGI();
	auto& desc = gi.GetTransitionDesc();
	
	// ���� �ִϸ��̼� ������Ʈ
	{
		//this->currentTime += Time::GetDeltaTime() * gi.GetPlaySpeed();
		auto currentAnimation = gi.GetCurrentAnimation();
		float duration = currentAnimation->GetDuration();
		int totalFrame = currentAnimation->GetTotalFrame();
		//if (currentAnimation->GetLoop() && currentTime >= duration)
		//{
		//	currentTime = 0.f;
		//}
		__int32 ratio = static_cast<__int32>(totalFrame / duration);

		//currentFrame = static_cast<__int32>(currentTime * ratio);
		//currentFrame = min(currentFrame, totalFrame - 1);

		//gi.SetCurrentFrame(currentFrame);
		//
		//gi.SetFrameRatio(static_cast<float>(currentTime - static_cast<float>(currentFrame) / ratio));

		
		desc.curr.sumTime += (desc.curr.speed * Time::GetDeltaTime());
		if (desc.curr.sumTime >= currentAnimation->GetDuration())
		{
			if (currentAnimation->GetLoop())
			{
				desc.curr.sumTime = 0.f;
			}
		}
		desc.curr.currFrame = static_cast<__int32>(desc.curr.sumTime * ratio);
		desc.curr.currFrame = min(static_cast<int>(desc.curr.currFrame), totalFrame - 1);
		desc.curr.nextFrame = min(static_cast<int>(desc.curr.currFrame+1), totalFrame -1);
		desc.curr.ratio = static_cast<float>(desc.curr.sumTime - static_cast<float>(desc.curr.sumTime) / ratio);
	}

	auto nextAnimation = gi.GetNextAnimation();
	// ���� �ִϸ��̼��� �ִٸ�
	if (nextAnimation)
	{
		float duration = nextAnimation->GetDuration();
		int totalFrame = nextAnimation->GetTotalFrame();
		__int32 ratio = static_cast<__int32>(totalFrame / duration);

		desc.transitionTotalTime += (Time::GetDeltaTime() * desc.transitionSpeed);
		desc.transitionRatio = desc.transitionTotalTime / desc.transitionDuration;
		// �ִϸ��̼� ��ü�� �Ϸ�ȴٸ�
		if (desc.transitionRatio >= 1.f)
		{
			desc.curr = desc.next;
			desc.ClearNextAnimation();
			gi.SetCurrentAnimation(nextAnimation);
			gi.SetNextAnimation(nullptr);
		}
		else
		{
			desc.next.sumTime += (desc.next.speed * Time::GetDeltaTime());
			if (desc.next.sumTime >= nextAnimation->GetDuration())
			{
				desc.next.sumTime = 0;
			}
			desc.next.currFrame = static_cast<__int32>(desc.next.sumTime * ratio);
			desc.next.currFrame = min(static_cast<int>(desc.next.currFrame), totalFrame - 1);
			desc.next.nextFrame = min(static_cast<int>(desc.next.currFrame + 1), totalFrame - 1);
			desc.next.ratio = static_cast<float>(desc.next.sumTime - static_cast<float>(desc.next.sumTime) / ratio);
		}
	}

	gi.SetTransitionDesc(desc);
}
