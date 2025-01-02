
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

    if(InKeyEvent.GetKey() == EKeys::F || InKeyEvent.GetKey() == EKeys::SpaceBar)
    {       
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

void UAPTutorialUI::OnSwitchingGuide()
{
    GuideOrder++;

    switch (GuideOrder)
    {
    case 1:
        OnFadeIn_GuideUI(); // Guide UI 애니메이션 재생생
        break;
    
    case 2:
        GuideUI->; // Guide UI 텍스트 재생생
        break;
    
    }
}