// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/APTitlePlayerController.h"

#include "UserInterface/Title/APTitleUI.h"
#include "UserInterface/Setting/APOptionSetting.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"

void AAPTitlePlayerController::BeginPlay()
{
    Super::BeginPlay();

    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    if(GI->IsNewGame())
    {
        CreateIntroUI();
    }
    else
    {
        CreateTitleUI();
    }
    
}

void AAPTitlePlayerController::CreateTitleUI()
{
    auto TitleUI = CreateWidget<UUserWidget>(this, TitleUIClass);

    if(TitleUI)
    {
        TitleUI->AddToViewport();

        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(TitleUI->GetCachedWidget());
        SetInputMode(Mode);
        bShowMouseCursor = true;
    }
}

void AAPTitlePlayerController::CreateIntroUI()
{
    auto IntroUI = CreateWidget<UUserWidget>(this, IntroUIClass);

    if(IntroUI)
    {
        IntroUI->AddToViewport();

        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(IntroUI->GetCachedWidget());
        SetInputMode(Mode);
        bShowMouseCursor = false;
    }

    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAPTitlePlayerController::RemoveIntroUI, IntroUI);
	GetWorld()->GetTimerManager().SetTimer(IntroeTimerHandle, TimerDelegate, IntroTime, false); // 임시로 인트로 삭제
}

void AAPTitlePlayerController::RemoveIntroUI(UUserWidget* IntroWidget)
{
    GetWorld()->GetTimerManager().ClearTimer(IntroeTimerHandle);
    IntroWidget->RemoveFromParent();
    UGameplayStatics::OpenLevel(GetWorld(), StartMap);
}

void AAPTitlePlayerController::OptionSetting()
{
    auto OptionSettingUI = Cast<UAPOptionSetting>(CreateWidget(this, OptionSettingClass)); if(!OptionSettingUI) return;
    OptionSettingUI->AddToViewport();
}
