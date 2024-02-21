#include "PlayerController/ArcanePunkPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "UserInterface/LoadingFade.h"
#include "UserInterface/Fade/HitFade.h"
#include "UserInterface/Status/APStatusUI.h"
#include "UserInterface/Skill/HomingTargetUI.h"
#include "UserInterface/Skill/APMouseClickBase.h"
#include "UserInterface/Setting/APOptionSetting.h"
#include "UserInterface/Save/APSaveSlotUI.h"
#include "UserInterface/Stage/APStageSelectingUI.h"
#include "UserInterface/Stage/APEntranceUI.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
    SmartKeyArr.Init(false, 4);
}

void AArcanePunkPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(GameAndUIInputMode);
    // CurrentMouseCursor = EMouseCursor::Default;
    bShowMouseCursor = true;

    StartLoading();
    LookStatus(); LookStatus(); // 오류 때문에 삽입
    
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
    InputComponent->BindAction(TEXT("FreeCameraMode"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::FreeCameraMode);
    InputComponent->BindAction(TEXT("Setting"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::OptionSetting);
}

void AArcanePunkPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

}

void AArcanePunkPlayerController::LookStatus()
{
    auto APCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!APCharacter) return; if(bFreeCameraMode) return;
    
	if(!bLookStatus)
	{
        bLookStatus = true;
		StatusWidget = Cast<UAPStatusUI>(CreateWidget(this, StatusWidgetClass));
        if(StatusWidget != nullptr)
        {
            StatusWidget->AddToViewport();
        }
	}
	else
	{
        bLookStatus = false;
        StatusWidget->RemoveFromParent();
	}
}

void AArcanePunkPlayerController::FreeCameraMode() // 수정 필요
{  
    if(!bFreeCameraMode)
    {
        MyCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!MyCharacter) return;
        FreeCamera = GetWorld()->SpawnActor<APawn>(FreeCameraClass, MyCharacter->ReturnCameraTransform()); if(!FreeCamera) return;
        
        Possess(FreeCamera);
        bFreeCameraMode = true;
    }
    else
    {
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
        
        for(AActor* Actor : Actors)
        {
            MyCharacter = Cast<AArcanePunkCharacter>(Actor);
        }
        if(!MyCharacter) return;
        bFreeCameraMode = false;
        Possess(MyCharacter);
        if(!FreeCamera) return;
        FreeCamera->Destroy();
    }
}

void AArcanePunkPlayerController::OptionSetting()
{
    if(OptionSettingUI) {OptionSettingUI->AddToViewport(); return;}
    else
    {
        OptionSettingUI = Cast<UAPOptionSetting>(CreateWidget(this, OptionSettingClass)); 
        if(!OptionSettingUI) return;

        OptionSettingUI->InitGraphicsSetting();
        OptionSettingUI->InitAudioSetting();
        OptionSettingUI->BindButton();
        OptionSettingUI->AddToViewport();
    }    
}

void AArcanePunkPlayerController::StartFadeIn()
{
    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass)); if(!FadeLoadingWidget) return;
    
    FadeLoadingWidget->AddToViewport();
    FadeLoadingWidget->FadeIn();

    if(!LoadingWidget) return;
    LoadingWidget->RemoveFromParent();
    GetWorldTimerManager().ClearTimer(LoadTimerHandle);
    
    CreateEntranceUI();
}

void AArcanePunkPlayerController::StartFadeOut()
{
    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass)); if(!FadeLoadingWidget) return;
    
    FadeLoadingWidget->AddToViewport();
    FadeLoadingWidget->FadeOut();
}

void AArcanePunkPlayerController::StartLoading()
{
    LoadingWidget = CreateWidget(this, LoadingWidgetClass); if(!LoadingWidget) return;
    
    LoadingWidget->AddToViewport();

    GetWorldTimerManager().SetTimer(LoadTimerHandle, this, &AArcanePunkPlayerController::StartFadeIn, LoadingTime, false);
}

void AArcanePunkPlayerController::CreateEntranceUI()
{
    auto EntranceUI = Cast<UAPEntranceUI>(CreateWidget(this, EntranceUIClass)); if(!EntranceUI) return;
    
    EntranceUI->AddToViewport();
}

void AArcanePunkPlayerController::OpenSaveSlot()
{
    if(SelectSlotUI) {SelectSlotUI->BindSlot(); SelectSlotUI->AddToViewport();}
    else
    {
        SelectSlotUI =  Cast<UAPSaveSlotUI>(CreateWidget(this, SelectSaveSlotClass)); if(!SelectSlotUI) return;
        SelectSlotUI->BindButton();
        SelectSlotUI->BindSlot();
        SelectSlotUI->AddToViewport();
    }
}

void AArcanePunkPlayerController::CloseSaveSlot()
{
    if(SelectSlotUI) SelectSlotUI->RemoveFromParent();
}

void AArcanePunkPlayerController::StartSaveUI()
{
    SaveUI = Cast<UUserWidget>(CreateWidget(this, SaveCompleteClass)); if(!SaveUI) return;

    SaveUI->AddToViewport();

    GetWorldTimerManager().SetTimer(SaveTimerHandle, this, &AArcanePunkPlayerController::EndSaveUI, LoadingTime, false);
}

void AArcanePunkPlayerController::OpenStageSelectingUI()
{
    auto StageSelectingUI = Cast<UAPStageSelectingUI>(CreateWidget<UUserWidget>(this, StageSelectingUIClasss));

    if(StageSelectingUI)
    {
        StageSelectingUI->BindButton();
        StageSelectingUI->AddToViewport();

        SetPause(true);
    }
}

void AArcanePunkPlayerController::CloseStageSelectingUI()
{
    SetPause(false);
}

void AArcanePunkPlayerController::EndSaveUI()
{
    SaveUI->RemoveFromParent();
}

void AArcanePunkPlayerController::HitUI()
{
    auto HitUI = Cast<UHitFade>(CreateWidget(this, HitWidgetClass)); if(!HitUI) return;
    
    HitUI->AddToViewport();
    HitUI->FadeOut();

    if(HitCS) ClientStartCameraShake(HitCS, 1.0f);
}

void AArcanePunkPlayerController::DisplayHomingUI(ESkillNumber SkillNumber)
{
    HomingUI = Cast<UHomingTargetUI>(CreateWidget(this, HomingUIClass)); if(!HomingUI) return;
    
    HomingUI->InputSkillInfo(SkillNumber);
    HomingUI->AddToViewport(); 
}

void AArcanePunkPlayerController::ReturnToDefault()
{
    CurrentMouseCursor = EMouseCursor::Default;
    auto& App = FSlateApplication::Get();
	App.SetAllUserFocusToGameViewport();
	App.QueryCursor();
}

void AArcanePunkPlayerController::PreventOtherClick(ESkillNumber SkillNumber)
{
    MouseClickUI = Cast<UAPMouseClickBase>(CreateWidget(this, MouseClickUIClass)); if(!MouseClickUI) return;
    
    MouseClickUI->InputSkillInfo(SkillNumber);
    MouseClickUI->AddToViewport(); 
}

void AArcanePunkPlayerController::SetHideUI(bool NewBool)
{
    if(NewBool)
    {
        if(!HideUI) HideUI = CreateWidget(this, HideUIClass); 
        if(HideUI) HideUI->AddToViewport(); 
    }
    else
    {
        if(HideUI) HideUI->RemoveFromParent();
    }
}
