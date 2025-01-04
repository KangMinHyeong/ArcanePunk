
#include "UserInterface/Tutorial/APTutorialUI.h"

#include "GameFramework/PlayerController.h"
#include "UserInterface/Conversation/APConversationUI.h"
#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"

void UAPTutorialUI::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerPC = GetOwningPlayer();
    OnFadeOut();
}

FReply UAPTutorialUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if(bAttackTutorial)
    {
        return Reply.Unhandled();
    }

    return Reply.Handled();
}

FReply UAPTutorialUI::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) 
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
    
    if(bAttackTutorial)
    {
        return Reply.Unhandled();
    }

	return Reply.Handled();
}

FReply UAPTutorialUI::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) 
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(bMoveTutorial)
    {      
        return StartMoveTutorial(Reply, InKeyEvent);
    }

    if(bAttackTutorial)
    {
        return Reply.Unhandled();
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

        SetIsFocusable(true);
        SetKeyboardFocus();
        break;

    case 5:
        OnFadeIn_TargetUI(); // 목표 UI 생성
        UAPDataTableSubsystem::SetTextBlock_Name(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Target, "Tutorial_Move"); 
        break;

    case 6:
        OnFadeOut_TargetUI(); // 목표 UI 닫기 및 이동 튜토리얼 종료
        OwnerPC->SetPause(true);
        bMoveTutorial = false;
        break; 

    case 7:
        OnFadeOut();  
        break; 
    
    case 8:
        GetOwningPlayerPawn()->EnableInput(OwnerPC.Get()); 
        OnFadeIn_GuideUI(); // Guide UI 애니메이션 재생
        GuideTextUI->InitOrder("Tutorial_2", true, this); // Guide UI 초기화
        break; 
        
    case 9:
        GuideTextUI->StartTextFlow(); // Guide UI 텍스트 재생
        break;

    case 10:
        OnFadeOut_GuideUI(); // 기본공격격 튜토리얼 시작 (Guide UI 애니메이션 재생)
        break;
    
    case 11:
        OnFadeIn(); // 이동 튜토리얼 시작 (보더 페이드 아웃 및 게임 퍼즈 해제)
        OwnerPC->SetPause(false);
        bAttackTutorial = true;

        SetIsFocusable(true);
        SetKeyboardFocus();
        break;  
    
    case 12:
        OnFadeIn_TargetUI(); // 목표 UI 생성
        UAPDataTableSubsystem::SetTextBlock_Name(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Target, "Tutorial_Attack"); 
        OnSpawnTutorialDummy.Broadcast();
        break;
    }
}

FReply UAPTutorialUI::StartMoveTutorial(const FReply & Reply, const FKeyEvent& InKeyEvent)
{
    auto returnValue = Reply.Handled();

    if(InKeyEvent.GetKey() == EKeys::W )
    {
        if(!TutorialData.bMove_W) 
        {
            TutorialData.bMove_W = true;
            TutorialData.MoveTutorialPassNum++;
        }
        returnValue = Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::A )
    {
        if(!TutorialData.bMove_A)
        {
            TutorialData.bMove_A = true;
            TutorialData.MoveTutorialPassNum++;
        }
        returnValue = Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::S )
    {
        if(!TutorialData.bMove_S)
        {
            TutorialData.bMove_S = true;
            TutorialData.MoveTutorialPassNum++;
        }
        returnValue = Reply.Unhandled();
    }
    if(InKeyEvent.GetKey() == EKeys::D )
    {
        if(!TutorialData.bMove_D)
        {
            TutorialData.bMove_D = true;
            TutorialData.MoveTutorialPassNum++;
        }
        returnValue = Reply.Unhandled();
    }

    if(TutorialData.MoveTutorialPassNum == 4)
    {
        if(!GetWorld()->GetTimerManager().IsTimerActive(TutorialTimerHandle))
        GetWorld()->GetTimerManager().SetTimer(TutorialTimerHandle, this, &UAPTutorialUI::StopPlayer, 2.4f, false);
    }

    return returnValue;
}

void UAPTutorialUI::StopPlayer()
{
    GetWorld()->GetTimerManager().ClearTimer(TutorialTimerHandle);
    GetOwningPlayerPawn()->DisableInput(OwnerPC.Get()); 
    GetWorld()->GetTimerManager().SetTimer(TutorialTimerHandle, this, &UAPTutorialUI::OnSwitchingGuide, 0.75f, false);
}