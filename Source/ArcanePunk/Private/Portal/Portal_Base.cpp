// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portal_Base.h"

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

	PortalInteractionTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("PortalInteractionTrigger"));
	PortalInteractionTrigger->SetupAttachment(PortalMesh); 

}

// Called when the game starts or when spawned
void APortal_Base::BeginPlay()
{
	Super::BeginPlay();

	InitHide(DefaultHidden);
	FLatentActionInfo LatentActionInfo;
	// UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Delay_Time + 0.5f, LatentActionInfo);
}

// void APortal_Base::OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	OverlapCharacter = Cast<AArcanePunkCharacter>(OtherActor);
// 	if(!OverlapCharacter) return;
// 	OverlapCharacter->SetCanMove(false);
// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PortalEffect, OverlapCharacter->GetActorLocation(), OverlapCharacter->GetActorRotation());
// 	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PortalSound, OverlapCharacter->GetActorLocation());
// 	GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &APortal_Base::StartTeleport, Delay_Time, false);	
// }

void APortal_Base::InitHide(bool IsHidden)
{
	SetActorHiddenInGame(IsHidden);
	
	if(IsHidden)
	{
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
	}
	else
	{
		SetActorEnableCollision(ECollisionEnabled::QueryOnly);
	}
}

void APortal_Base::StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint)
{
	Character->SetActorLocation(TeleportPoint);
	Character->SetCanMove(true);
	GetWorldTimerManager().ClearTimer(Delay_TimerHandle);
}

void APortal_Base::SpawnSound(FVector Location)
{
	auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

	float SoundVolume = GI->GameSoundVolume.MasterVolume * GI->GameSoundVolume.EffectVolume;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PortalSound, Location, FRotator::ZeroRotator, SoundVolume, 1.0f, SoundStartTime);
}
