#include "PlayerController/ArcanePunkPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "UserInterface/Fade/LoadingFade.h"
#include "UserInterface/Fade/HitFade.h"
#include "UserInterface/HUD/Status/APStatusUI.h"
#include "UserInterface/Setting/APOptionSetting.h"
#include "UserInterface/Save/APSaveSlotUI.h"
#include "UserInterface/Stage/APStageSelectingUI.h"
#include "UserInterface/Stage/APEntranceUI.h"
#include "Interfaces/InteractionInterface.h"
#include "UserInterface/Conversation/APConversationUI.h"
#include "GameMode/APGameModeBase.h"
#include "UserInterface/Shop/APShoppingUI.h"
#include "UserInterface/Inform/Skill/APSkillWindow.h"
#include "UserInterface/Common/DisplayOnly/APScreenEffect.h"
#include "Interaction/APInteraction_Shop.h"
#include "UserInterface/HUD/APHUD.h"
#include "UserInterface/Dead/APDeadUI.h"
#include "GameMode/APGameModeTutorialStage.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameInstance/APGameInstance.h"
#include "Components/Character/APAttackComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
}

void AArcanePunkPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
    InputComponent->BindAction(TEXT("Setting"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::OptionSetting);
    InputComponent->BindAction(TEXT("SkillWindow"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::DisplaySkillWindow);
    InputComponent->BindAction(TEXT("Save"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::OpenSaveSlot);
    InputComponent->BindAction(TEXT("ChangePlayer_0"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::SwapPlayer_0);
    InputComponent->BindAction(TEXT("ChangePlayer_1"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::SwapPlayer_1);
    InputComponent->BindAction(TEXT("ChangePlayer_2"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::SwapPlayer_2);
}

void AArcanePunkPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if(bSwap)
    {
        SwapSlowTime += DeltaTime;
        SwapSlow = FMath::FInterpConstantTo(SwapSlow, 0.5f, DeltaTime, SwapDelayTime * 2.0f);
        
        if(SwapSlowTime >= SwapDelayTime)
        {
            SwapSlowTime = 0.0f;
            bSwap = false;
            SwapSlow = 1.0f;
        }

        GetWorld()->GetWorldSettings()->SetTimeDilation(SwapSlow);
    }
}

void AArcanePunkPlayerController::InitMainPlayer(AActor* PlayerStart)
{
    TArray<uint8> PlayerUIID;
	for(auto Class : PlayerClass)
	{
		auto MainPlayer = GetWorld()->SpawnActor<AArcanePunkCharacter>(Class, FVector(10000.0f,10000.0f,10000.0f), FRotator::ZeroRotator);
		MainPlayer->SetActorHiddenInGame(true);  // 화면에서 숨김
        MainPlayer->SetActorEnableCollision(false);  // 충돌 비활성화
        MainPlayer->SetActorTickEnabled(false);  // 틱 비활성화	
        
        PlayerUIID.Emplace(MainPlayer->CharacterUIID);
		Players.Emplace(MainPlayer);
	}

    GetHUD<AAPHUD>()->Init(PlayerUIID);
	
	uint8 PlayerIndex = 0; 
	for(auto MainPlayer : Players)
	{
		GetHUD<AAPHUD>()->BindStatusBar(Cast<AArcanePunkCharacter>(MainPlayer), PlayerIndex++);
    }
    
    SwapPlayer(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

    SetMouseCursor();
    StartLoading();
    LookStatus(); LookStatus(); // 오류 때문에 삽입
}

void AArcanePunkPlayerController::SwapMainPlayerByIndex(uint8 Index)
{
    if(MainPlayerIndex == Index) return;
    if(!CheckSwapPlayerCondition(Index))
    {
        UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), CannotChange, true, true); 
        return;
    }

    float Gauge = MyCharacter->GetSwapGauge();
    bEnhanceSwap = false;
    if(Gauge >= 100.0f)
    {
        bEnhanceSwap = true;
        GetHUD<AAPHUD>()->ClearSwapGauge(MainPlayerIndex);
        MyCharacter->SetSwapGauge(0.0f);
        Gauge = 0.0f;
    }
    SwapEnemy = DetectEnemy(MyCharacter.Get());
    auto PriorCharacter = MyCharacter;
    // UnPossess();    
    PriorCharacter->SetActorEnableCollision(false);  // 충돌 비활성화
    PriorCharacter->SwapMainPlayer(false, bEnhanceSwap, SwapEnemy);
    PriorCharacter->RestoreSkills();
    PriorCharacter->DisableInput(this);

    bSkilling = PriorCharacter->IsSkilling();
    // SetSwitchingLocation(PriorCharacter.Get(), SwitchingLocation, SwitchingRotation);
    
    GetHUD<AAPHUD>()->DeactivateStatusBar(MainPlayerIndex, Gauge);

    MainPlayerIndex = Index;
    SwapPlayer(PriorCharacter->GetActorLocation(), PriorCharacter->GetActorRotation());
}

AActor* AArcanePunkPlayerController::DetectEnemy(AArcanePunkCharacter* CurrentPlayer)
{
    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_CharacterBase::StaticClass(), Enemies);

    float Dist;
    auto Enemy = UGameplayStatics::FindNearestActor(CurrentPlayer->GetActorLocation(), Enemies, Dist);

    float AttackRange = CurrentPlayer->GetAttackComponent()->GetBaseAttackDist();
    if(Dist <= AttackRange)
    {
        return Enemy;
    }

    return nullptr;
}

bool AArcanePunkPlayerController::CheckSwapPlayerCondition(uint8 Index)
{
    if(!MyCharacter->CheckSwapPlayerCondition()) return false;
    if(!GetHUD<AAPHUD>()->CheckSwapCoolDown(Index)) return false;

    return true;
}

void AArcanePunkPlayerController::SwapPlayer(const FVector & Loc, const FRotator & Rot)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SwitchingEffect, Loc + FVector(0.0f, 0.0f, 50.0f), Rot);

    if (MainPlayerIndex < Players.Num())
    {		
		MyCharacter = Cast<AArcanePunkCharacter>(Players[MainPlayerIndex]);
        Possess(MyCharacter.Get());   

        GetHUD<AAPHUD>()->ActivateStatusBar(MainPlayerIndex);
        GetHUD<AAPHUD>()->MainPlayerIndex = MainPlayerIndex;

        MyCharacter->SetActorHiddenInGame(false); 
        MyCharacter->SetActorTickEnabled(true);  // 틱 활성화
        MyCharacter->SetActorLocation(Loc);
        MyCharacter->SetActorRotation(Rot);
        MyCharacter->ChangePlayer(this); 
        MyCharacter->SetActorEnableCollision(true);  // 충돌 활성화
        MyCharacter->EnableInput(this);
        MyCharacter->SwapMainPlayer(true, bEnhanceSwap, SwapEnemy, bSkilling);

        bSwap = true;
    }
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
}

void AArcanePunkPlayerController::LookStatus()
{ 
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

void AArcanePunkPlayerController::OptionSetting()
{
    if(!OptionSettingUI.IsValid())
    {
        OptionSettingUI = Cast<UAPOptionSetting>(CreateWidget(this, OptionSettingClass)); 
    }
    
    if(OptionSettingUI.IsValid())
    {
        OptionSettingUI->AddToViewport();
        SetPause(true);
    }
}

void AArcanePunkPlayerController::StartFadeIn(float MultipleSpeed, bool bEntrance)
{
    if(!FadeLoadingWidget.IsValid())
    {
        FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass)); if(!FadeLoadingWidget.IsValid()) return;
    }

    if(!FadeLoadingWidget->IsInViewport()) FadeLoadingWidget->AddToViewport();

    FadeLoadingWidget->FadeIn(MultipleSpeed);

    if(MyCharacter.IsValid()) MyCharacter->EnableInput(this); 
    
    if(bEntrance)
    {
        auto TutorialGM = Cast<AAPGameModeTutorialStage>(UGameplayStatics::GetGameMode(GetWorld()));
        if(TutorialGM) TutorialGM->StartTutorial(this);

        if(!LoadingWidget.IsValid()) return;
        LoadingWidget->RemoveFromParent();
        GetWorldTimerManager().ClearTimer(LoadTimerHandle);
                
        // CreateEntranceUI();
    }
}

void AArcanePunkPlayerController::StartFadeOut(float MultipleSpeed, bool bEntrance)
{
    if(!FadeLoadingWidget.IsValid())
    {
        FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass)); if(!FadeLoadingWidget.IsValid()) return;
    }

    FadeLoadingWidget->AddToViewport();
    
    if(FadeLoadingWidget.IsValid()) FadeLoadingWidget->FadeOut(MultipleSpeed, bEntrance);
}

void AArcanePunkPlayerController::StartLoading()
{
    if(MyCharacter.IsValid()) MyCharacter->DisableInput(this); 

    LoadingWidget = CreateWidget(this, LoadingWidgetClass); if(!LoadingWidget.IsValid()) return;
    
    LoadingWidget->AddToViewport();

    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AArcanePunkPlayerController::StartFadeIn, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, TimerDelegate, LoadingTime, false);
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

    if(!MyCharacter.IsValid()) return;
    MyCharacter->DisableInput(this); 
    SetPause(true);
}

void AArcanePunkPlayerController::CloseSaveSlot()
{
    if(SelectSlotUI.IsValid()) SelectSlotUI->RemoveFromParent();
    OpenSaveSlot();

    if(!MyCharacter.IsValid()) return;
    MyCharacter->EnableInput(this); 
    SetPause(false);
}

void AArcanePunkPlayerController::OpenStageSelectingUI(AActor* CameraActor)
{
    if(!MyCharacter.IsValid()) return;
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
    SetPause(false);
    if(MyCharacter.IsValid()) {MyCharacter->SetActorHiddenInGame(false); SetViewTargetWithBlend(MyCharacter.Get(), BlendTime); MyCharacter->EnableInput(this);} 
}

void AArcanePunkPlayerController::OpenConversationUI(AActor* CameraActor, const int32 GroupID)
{
    if(!MyCharacter.IsValid()) return;
    // MyCharacter->SetActorHiddenInGame(true); 
    MyCharacter->DisableInput(this); 
    MyCharacter->GetAPHUD()->SetStatusVisibility(true);
    ConversationActor = CameraActor;
    // SetViewTargetWithBlend(CameraActor, BlendTime);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AArcanePunkPlayerController::OnConversationUI, GroupID);
	GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, TimerDelegate, BlendTime, false);
}

void AArcanePunkPlayerController::OnConversationUI(const int32 GroupID)
{
    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
    ConversationUI = CreateWidget<UAPConversationUI>(GetWorld(), ConversationUIClass); if(!ConversationUI.IsValid()) return;
    ConversationUI->AddToViewport();
    ConversationUI->InitOrder(GroupID);

    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GM) GM->OnEndedConversation.AddDynamic(this, &AArcanePunkPlayerController::CloseConversationUI);
}

void AArcanePunkPlayerController::CloseConversationUI(const int32 GroupID)
{
    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GM) GM->OnEndedConversation.RemoveDynamic(this, &AArcanePunkPlayerController::CloseConversationUI);

    if(ConversationUI.IsValid()) ConversationUI->RemoveFromParent();
    if(MyCharacter.IsValid()) 
    {   
        // MyCharacter->SetActorHiddenInGame(false); 
        // SetViewTargetWithBlend(MyCharacter.Get(), BlendTime); 
        MyCharacter->GetAPHUD()->SetStatusVisibility(false);
        MyCharacter->EnableInput(this);
        MyCharacter->OnEndInteract.Broadcast(MyCharacter.Get());
    } 

    // GroupID가 상점 번호일 때
    // if(GroupID != 1000) return;
    // auto ShopActor = Cast<AAPInteraction_Shop>(ConversationActor);
    // if(ShopActor) {ShopActor->OpenShopUI();}
}

void AArcanePunkPlayerController::OpenShoppingUI(AActor* ShopActor, const FShopListData & ShopListData)
{
    ShoppingUI = Cast<UAPShoppingUI>(CreateWidget<UUserWidget>(this, ShoppingUIClass));
    if(ShoppingUI.IsValid())
    {
        ShoppingUI->InitShopData(ShopActor, ShopListData);
        ShoppingUI->AddToViewport();
        
        if(!MyCharacter.IsValid()) return;
        MyCharacter->DisableInput(this); 
        MyCharacter->GetAPHUD()->SetStatusVisibility(true);
    }
}

void AArcanePunkPlayerController::CloseShoppingUI()
{
    if(MyCharacter.IsValid() && ShoppingUI.IsValid()) 
    {
        MyCharacter->SetActorHiddenInGame(false);
        MyCharacter->EnableInput(this);
        ShoppingUI->RemoveFromParent();
        MyCharacter->GetAPHUD()->SetStatusVisibility(false);
        MyCharacter->OnEndInteract.Broadcast(MyCharacter.Get());
    } 
}

void AArcanePunkPlayerController::HitUI()
{
    auto HitUI = Cast<UHitFade>(CreateWidget(this, HitWidgetClass)); if(!HitUI) return;
    
    HitUI->AddToViewport();
    HitUI->FadeOut();
    
    if(HitCS) ClientStartCameraShake(HitCS, 1.0f);
}

void AArcanePunkPlayerController::ReturnToDefault()
{
    CurrentMouseCursor = EMouseCursor::Default;
    auto& App = FSlateApplication::Get();
	App.SetAllUserFocusToGameViewport();
	App.QueryCursor();
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

void AArcanePunkPlayerController::DisplayDeadUI()
{
    auto DeadUI = CreateWidget<UUserWidget>(GetWorld(), DeadUIClass); if(!DeadUI) return;
    DeadUI->AddToViewport();
}

void AArcanePunkPlayerController::DisplaySkillWindow()
{
	auto SkillWindowUI = CreateWidget<UAPSkillWindow>(GetWorld(), SkillWindowUIClass);
	if(SkillWindowUI) SkillWindowUI->AddToViewport();
}

void AArcanePunkPlayerController::ParryingCameraShake()
{
    if(ParryingCS) ClientStartCameraShake(ParryingCS);

    auto ParryUI = CreateWidget<UAPScreenEffect>(GetWorld(), ParryScreenEffectClass); if(!ParryUI) return;
	ParryUI->OnScreenEffect();
    ParryUI->AddToViewport();
}

void AArcanePunkPlayerController::AttackCameraShake()
{
    if(AttackCS) ClientStartCameraShake(AttackCS);
}

void AArcanePunkPlayerController::DisplayEnding()
{
    if(!MyCharacter.IsValid()) return;
    MyCharacter->DisableInput(this); 

    auto EndUI = CreateWidget<UAPDeadUI>(GetWorld(), DemoEndUIClass); if(!EndUI) return;
    EndUI->AddToViewport();
}
