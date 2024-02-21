// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/LevelPoratl.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UserInterface/LoadingFade.h"
#include "TimerManager.h"
#include "GameInstance/APGameInstance.h"

ALevelPoratl::ALevelPoratl()
{
}

void ALevelPoratl::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelPoratl::OnTeleport_A(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor);
    if(!Character) return;

    CharacterPC = Cast<AArcanePunkPlayerController>(Character->GetController());
    if(!CharacterPC) return;
    CharacterPC->StartFadeOut();

    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(CharacterPC, FadeLoadingWidgetClass));
    if(!FadeLoadingWidget) return;

    GetWorldTimerManager().SetTimer(LoadTimerHandle, this, &ALevelPoratl::Loading, FadeLoadingWidget->FadeTime - 0.1f, false);

    Test_StageUnlocking();
}

void ALevelPoratl::Loading()
{
    if(!CharacterPC) return;
    CharacterPC->StartLoading();

    UGameplayStatics::OpenLevel(CharacterPC, NextLevel);

    GetWorldTimerManager().ClearTimer(LoadTimerHandle);
}

void ALevelPoratl::Test_StageUnlocking()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    if(GI->CanEnterStage.Contains(UnlockedStageName)) GI->CanEnterStage[UnlockedStageName] = true;
}

