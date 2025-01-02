
#include "UserInterface/Tutorial/APTutorialUI.h"

#include "GameFramework/PlayerController.h"

void UAPTutorialUI::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerPC = GetOwningPlayer();
    OnFadeOut();
}

FReply UAPTutorialUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        return Reply.Handled();
    }

    return Reply.Handled();
}

FReply UAPTutorialUI::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) // 전부 인벤토리로 (F키)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(bMove) return Reply.UnHandled();
    
	return Reply.Handled();
}

void UAPTutorialUI::OnSwitchingGuide()
{
    GuideOrder++;

    switch (GuideOrder)
    {
    case 1:
        OnFadeIn_GuideUI(); // Guide UI 애니메이션 재생
        break;
    
    case 2:
        GuideUI->InitOrder("Tutorial_1", true); // Guide UI 텍스트 재생생
        break;
    
    case 3:
        OnFadeOut_GuideUI(); // 이동 튜토리얼 시작 (Guide UI 애니메이션 재생)
        break;
    
    case 3:
        OnFadeIn(); // 이동 튜토리얼 시작 (보더 페이드 아웃 및 게임 퍼즈 해제)
        OwnerPC->SetPause(false);
        bMove = true;
        break;
    }
}