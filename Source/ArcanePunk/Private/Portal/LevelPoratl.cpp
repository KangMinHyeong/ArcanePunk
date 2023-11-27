// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/LevelPoratl.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UserInterface/LoadingFade.h"
#include "TimerManager.h"

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
}

void ALevelPoratl::Loading()
{
    UGameplayStatics::OpenLevel(CharacterPC, NextLevel);

    if(!CharacterPC) return;
    CharacterPC->StartLoading();

    GetWorldTimerManager().ClearTimer(LoadTimerHandle);
}

