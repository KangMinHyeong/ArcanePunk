
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

void UAPTutorialUI::OnPlayingGuide_1()
{

}

void UAPTutorialUI::OnPlayingGuide_1()
{
    
}
