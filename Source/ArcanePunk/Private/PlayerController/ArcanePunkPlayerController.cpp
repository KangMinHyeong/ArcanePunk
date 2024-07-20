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
#include "UserInterface/Interaction/InteractionWidget.h"
#include "Interfaces/InteractionInterface.h"
#include "UserInterface/Conversation/APConversationUI.h"
#include "ArcanePunk/APGameModeBase.h"
#include "UserInterface/Shop/APShoppingUI.h"
#include "UserInterface/Inform/APSkillWindow.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
    SmartKeyArr.Init(false, 4);
}

void AArcanePunkPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetMouseCursor();
    
   
    StartLoading();
    LookStatus(); LookStatus(); // 오류 때문에 삽입
    
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
    InputComponent->BindAction(TEXT("FreeCameraMode"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::FreeCameraMode);
    InputComponent->BindAction(TEXT("Setting"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::OptionSetting);
    InputComponent->BindAction(TEXT("SkillWindow"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::DisplaySkillWindow);
}

void AArcanePunkPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

}

void AArcanePunkPlayerController::SetMouseCursor()
{
    SetInputMode(GameAndUIInputMode);
     
    bShowMouseCursor = true;

    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;
    if(GM->IsBattleStage())
    {
        CurrentMouseCursor = EMouseCursor::CardinalCross;
    }
    else
    {
        CurrentMouseCursor = EMouseCursor::Default;
    }
    // 
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
        MyCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!MyCharacter.IsValid()) return;
        FreeCamera = GetWorld()->SpawnActor<APawn>(FreeCameraClass, MyCharacter->ReturnCameraTransform()); if(!FreeCamera.IsValid()) return;
        
        Possess(FreeCamera.Get());
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
        if(!MyCharacter.IsValid()) return;
        bFreeCameraMode = false;
        Possess(MyCharacter.Get());
        if(!FreeCamera.IsValid()) return;
        FreeCamera->Destroy();
    }
}

void AArcanePunkPlayerController::OptionSetting()
{
    auto OptionSettingUI = Cast<UAPOptionSetting>(CreateWidget(this, OptionSettingClass)); if(!OptionSettingUI) return;
    OptionSettingUI->AddToViewport();
}

void AArcanePunkPlayerController::StartFadeIn()
{
    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass)); if(!FadeLoadingWidget) return;
    
    FadeLoadingWidget->AddToViewport();
    FadeLoadingWidget->FadeIn();

    if(!LoadingWidget.IsValid()) return;
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
    LoadingWidget = CreateWidget(this, LoadingWidgetClass); if(!LoadingWidget.IsValid()) return;
    
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
    if(SelectSlotUI.IsValid()) {SelectSlotUI->BindSlot(); SelectSlotUI->AddToViewport();}
    else
    {
        SelectSlotUI =  Cast<UAPSaveSlotUI>(CreateWidget(this, SelectSaveSlotClass)); if(!SelectSlotUI.IsValid()) return;
        SelectSlotUI->BindButton();
        SelectSlotUI->BindSlot();
        SelectSlotUI->AddToViewport();
    }
}

void AArcanePunkPlayerController::CloseSaveSlot()
{
    if(SelectSlotUI.IsValid()) SelectSlotUI->RemoveFromParent();
    OpenSaveSlot();
}

void AArcanePunkPlayerController::StartSaveUI()
{
    SaveUI = Cast<UUserWidget>(CreateWidget(this, SaveCompleteClass)); if(!SaveUI.IsValid()) return;

    SaveUI->AddToViewport();
}

void AArcanePunkPlayerController::OpenStageSelectingUI(AActor* CameraActor)
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    MyCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!MyCharacter.IsValid()) return;
    MyCharacter->SetActorHiddenInGame(true); MyCharacter->DisableInput(this); 
    SetViewTargetWithBlend(CameraActor, BlendTime);
    
	GetWorldTimerManager().SetTimer(InteractionTimerHandle, this, &AArcanePunkPlayerController::OnStageSelectingUI, BlendTime+0.75f, false);    
}

void AArcanePunkPlayerController::OnStageSelectingUI()
{
    GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
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
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Visible);
    SetPause(false);
    if(MyCharacter.IsValid()) {MyCharacter->SetActorHiddenInGame(false); SetViewTargetWithBlend(MyCharacter.Get(), BlendTime); MyCharacter->EnableInput(this);} 
}

void AArcanePunkPlayerController::OpenConversationUI(AActor* CameraActor, FName Name, uint8 State)
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);

    MyCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!MyCharacter.IsValid()) return;
    MyCharacter->SetActorHiddenInGame(true); MyCharacter->DisableInput(this); 
    SetViewTargetWithBlend(CameraActor, BlendTime);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AArcanePunkPlayerController::OnConversationUI, Name, State);
	GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, TimerDelegate, BlendTime, false);
}

void AArcanePunkPlayerController::OnConversationUI(FName Name, uint8 State)
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);

    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
    ConversationUI = CreateWidget<UAPConversationUI>(GetWorld(), ConversationUIClass); if(!ConversationUI.IsValid()) return;
    ConversationUI->AddToViewport();
    ConversationUI->InitOrder(Name, State);
}

void AArcanePunkPlayerController::CloseConversationUI()
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Visible);
    if(ConversationUI.IsValid()) ConversationUI->RemoveFromParent();
    if(MyCharacter.IsValid()) {MyCharacter->SetActorHiddenInGame(false); SetViewTargetWithBlend(MyCharacter.Get(), BlendTime); MyCharacter->EnableInput(this);} 
}

void AArcanePunkPlayerController::OpenShoppingUI(AActor* ShopActor, FShopListData ShopListData)
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    ShoppingUI = Cast<UAPShoppingUI>(CreateWidget<UUserWidget>(this, ShoppingUIClass));

    if(ShoppingUI.IsValid())
    {
        ShoppingUI->InitShopData(ShopActor, ShopListData);
        ShoppingUI->AddToViewport();
        
        MyCharacter = Cast<AArcanePunkCharacter>(GetPawn()); if(!MyCharacter.IsValid()) return;
        MyCharacter->DisableInput(this); 
    }
}

void AArcanePunkPlayerController::CloseShoppingUI()
{
    if(InteractionWidget.IsValid()) InteractionWidget->SetVisibility(ESlateVisibility::Visible);

    if(MyCharacter.IsValid() && ShoppingUI.IsValid()) 
    {
        MyCharacter->SetActorHiddenInGame(false);
        MyCharacter->EnableInput(this);
        ShoppingUI->RemoveFromParent();
    } 
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
    HomingUI = Cast<UHomingTargetUI>(CreateWidget(this, HomingUIClass)); if(!HomingUI.IsValid()) return;
    
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

void AArcanePunkPlayerController::PreventOtherClick(ESkillNumber SkillNumber, bool Loop)
{
    if(!MouseClickUI.IsValid())
    {
        MouseClickUI = Cast<UAPMouseClickBase>(CreateWidget(this, MouseClickUIClass)); if(!MouseClickUI.IsValid()) return;    
    }
    
    MouseClickUI->InputSkillInfo(SkillNumber, Loop);
    MouseClickUI->AddToViewport(); 
}

void AArcanePunkPlayerController::PreventOtherClick_Ult(bool Loop)
{
    if(!MouseClickUI.IsValid())
    {
        MouseClickUI = Cast<UAPMouseClickBase>(CreateWidget(this, MouseClickUIClass)); if(!MouseClickUI.IsValid()) return;   
    }
    
    MouseClickUI->InputSkillInfo_Ult(Loop);
    MouseClickUI->AddToViewport(); 
}

void AArcanePunkPlayerController::SetHideUI(bool NewBool)
{
    if(NewBool)
    {
        if(!HideUI.IsValid()) HideUI = CreateWidget(this, HideUIClass); 
        if(HideUI.IsValid()) HideUI->AddToViewport(); 
    }
    else
    {
        if(HideUI.IsValid()) HideUI->RemoveFromParent();
    }
}

void AArcanePunkPlayerController::OpenInteraction(AActor* NewActor, FInteractData InteractData)
{
    if(InteractionActor == NewActor) return;

    if(InteractionWidget.IsValid()) InteractionWidget->RemoveFromParent();
    InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionUIClass); if(!InteractionWidget.IsValid()) return;
    InteractionWidget->AddToViewport();
    InteractionWidget->UpdateWidget(InteractData);
    InteractionActor = NewActor;
}

void AArcanePunkPlayerController::CloseInteraction(AActor* NewActor)
{
    if(InteractionActor == NewActor)
    {
        if(InteractionWidget.IsValid()) InteractionWidget->RemoveFromParent();
        InteractionActor = nullptr;
    }
}

void AArcanePunkPlayerController::DisplayDeadUI()
{
    auto DeadUI = CreateWidget<UUserWidget>(GetWorld(), DeadUIClass); if(!DeadUI) return;
    DeadUI->AddToViewport();
}

void AArcanePunkPlayerController::DisplayNotEnoughMPUI()
{
    auto NotEnoughMPUI = CreateWidget<UUserWidget>(GetWorld(), NotEnoughMPUIClass); if(!NotEnoughMPUI) return;
    NotEnoughMPUI->AddToViewport();
}

void AArcanePunkPlayerController::DisplaySkillWindow()
{
	auto SkillWindowUI = CreateWidget<UAPSkillWindow>(GetWorld(), SkillWindowUIClass);
	if(SkillWindowUI) SkillWindowUI->AddToViewport();
}