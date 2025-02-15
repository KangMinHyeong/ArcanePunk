// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Portal/LevelPoratl.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UserInterface/Fade/LoadingFade.h"
#include "TimerManager.h"
#include "GameInstance/APGameInstance.h"
#include "Components/Common/APInteractionBoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ALevelPoratl::ALevelPoratl()
{
}

void ALevelPoratl::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelPoratl::Interact(AArcanePunkCharacter *PlayerCharacter)
{
	FVector PlayerLocation = PlayerCharacter->GetMesh()->GetComponentLocation();
	PlayerCharacter->SetCanMove(false);
	UNiagaraFunctionLibrary::SpawnSystemAttached(PortalEffect, PlayerCharacter->GetMesh(), TEXT("PortalEffect"), PlayerLocation, PlayerCharacter->GetMesh()->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
	UAPSoundSubsystem::SpawnEffectSoundAtLocation(UAPGameInstance::GetSoundGI(GetWorld()) , PortalSound, PlayerLocation);

	PlayerCharacter->GetPlayerPanelAura()->SetHiddenInGame(true);

    GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &ALevelPoratl::StartLoading, Delay_Time, false);

    CharacterPC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!CharacterPC.IsValid()) return;
    PlayerCharacter->DisableInput(CharacterPC.Get());

}

void ALevelPoratl::StartLoading()
{
    if(!CharacterPC.IsValid()) return;
    CharacterPC->StartFadeOut();

    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(CharacterPC.Get(), FadeLoadingWidgetClass));
    if(!FadeLoadingWidget) return;

    GetWorldTimerManager().SetTimer(LoadTimerHandle, this, &ALevelPoratl::Loading, 1.1f, false);

    Test_StageUnlocking();
}

void ALevelPoratl::Loading()
{
    if(!CharacterPC.IsValid()) return;
    CharacterPC->StartLoading();

    UGameplayStatics::OpenLevel(CharacterPC.Get(), NextLevel);

    GetWorldTimerManager().ClearTimer(LoadTimerHandle);
}

void ALevelPoratl::Test_StageUnlocking()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    if(GI->GetCanEnterStage().Contains(UnlockedStageName)) GI->SetCanEnterStage(UnlockedStageName, true);
}

