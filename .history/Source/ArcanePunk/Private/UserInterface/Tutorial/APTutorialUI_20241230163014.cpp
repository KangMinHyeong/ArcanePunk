
#include "UserInterface/Tutorial/APTutorialUI.h"

#include "GameFramework/PlayerController.h"
#include "UserInterface/Conversation/APConversationUI.h"

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

    if(bMoveTutorial)
    {      
        return StartMoveTutorial();
    }
    
	return Reply.Handled();
}

void UAPTutorialUI::OnSwitchingGuide()
{
    GetWorld()->GetTimerManager().ClearTimer(TutorialTimerHandle);
    
    GuideOrder++;
    UE_LOG(LogTemp, Display, TEXT("Your GuideOrder %d"), GuideOrder);

    switch (GuideOrder)
    {
    case 1:
        OnFadeIn_GuideUI(); // Guide UI 애니메이션 재생
        GuideTextUI->InitOrder("Tutorial_1", true, this); // Guide UI 초기화
        break;
    
    case 2:
        GuideTextUI->StartTextFlow(); // Guide UI 텍스트 재생
        break;

    case 3:
        OnFadeOut_GuideUI(); // 이동 튜토리얼 시작 (Guide UI 애니메이션 재생)
        break;
    
    case 4:
        OnFadeIn(); // 이동 튜토리얼 시작 (보더 페이드 아웃 및 게임 퍼즈 해제)
        OwnerPC->SetPause(false);
        bMoveTutorial = true;
        break;

    case 5:
        OnFadeIn_TargetUI(); // 목표 UI 생성
        break;

    case 6:
        OnFadeOut_TargetUI(); // 목표 UI 닫기
        break;   
    }
}

FReply UAPTutorialUI::StartMoveTutorial()
{
    if(InKeyEvent.GetKey() == EKeys::W )
    {
        if(!TutorialData.bMove_W) 
        {
            TutorialData.bMove_W = true;
            TutorialData.MoveTutorialPassNum++;
        }
        return Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::A )
    {
        if(!TutorialData.bMove_A)
        {
            TutorialData.bMove_A = true;
            TutorialData.MoveTutorialPassNum++;
        }
        return Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::S )
    {
        if(!TutorialData.bMove_S)
        {
            TutorialData.bMove_S = true;
            TutorialData.MoveTutorialPassNum++;
        }
        return Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::D )
    {
        if(!TutorialData.bMove_D)
        {
            TutorialData.bMove_D = true;
            TutorialData.MoveTutorialPassNum++;
        }
        return Reply.Unhandled();
    }

    if(TutorialData.MoveTutorialPassNum == 4)
    {
        GetWorld()->GetTimerManager().SetTimer(TutorialTimerHandle, this, &UAPTutorialUI::OnSwitchingGuide, 4.0f, false);
    }

    return Reply.Handled();
}