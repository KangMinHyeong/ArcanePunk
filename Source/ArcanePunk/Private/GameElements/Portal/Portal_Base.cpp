// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Portal/Portal_Base.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Common/APInteractionBoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameInstance/APGameInstance.h"

// Sets default values
APortal_Base::APortal_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	SetRootComponent(PortalMesh);

	PortalEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalEffectComp"));
	PortalEffectComp->SetupAttachment(PortalMesh); 

	PortalInteractionTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("PortalInteractionTrigger"));
	PortalInteractionTrigger->SetupAttachment(PortalMesh); 

}

// Called when the game starts or when spawned
void APortal_Base::BeginPlay()
{
	Super::BeginPlay();
	APGI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!APGI.IsValid()) return;
	PortalEffectComp->DeactivateImmediate();
	InitHide(DefaultHidden);
	FLatentActionInfo LatentActionInfo;
	// UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Delay_Time + 0.5f, LatentActionInfo);
}

void APortal_Base::InitHide(bool IsHidden, float DelayTime)
{
	if(IsHidden)
	{
		SetActorHiddenInGame(IsHidden);
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
	}
	else
	{
		if(DelayTime > KINDA_SMALL_NUMBER)
		{
			GetWorld()->GetTimerManager().SetTimer(HiddenTimerHandle, this, &APortal_Base::DelayHidden, DelayTime, false);
		}
		else {DelayHidden();}
	}
}

void APortal_Base::StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint)
{
	Character->SetActorLocation(TeleportPoint);
	Character->SetCanMove(true);
	Character->GetPlayerPanelAura()->SetHiddenInGame(false);

	Character->SetActorEnableCollision(true);

	GetWorldTimerManager().ClearTimer(Delay_TimerHandle);
}

void APortal_Base::SpawnSound(FVector Location)
{
	float SoundVolume = APGI->GetGameSoundVolume().MasterVolume * APGI->GetGameSoundVolume().EffectVolume;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PortalSound, Location, FRotator::ZeroRotator, SoundVolume, 1.0f, SoundStartTime);
}

void APortal_Base::DelayHidden()
{
	GetWorld()->GetTimerManager().ClearTimer(HiddenTimerHandle);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(ECollisionEnabled::QueryOnly);
	PortalEffectComp->Activate();
}
