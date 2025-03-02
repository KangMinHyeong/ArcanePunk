// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"

// Sets default values
AEnemy_DropBase::AEnemy_DropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("InteractionTrigger"));
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	DropTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropTrailEffect"));

	SetRootComponent(DropMesh);
	InteractionTrigger->SetupAttachment(DropMesh);
	DropTrailEffect->SetupAttachment(DropMesh);


	// DropMesh->SetSimulatePhysics(true);
	// DropMesh->BodyInstance.bLockXRotation = true;
	// DropMesh->BodyInstance.bLockYRotation = true;
	// DropMesh->SetPhysicsMaxAngularVelocityInDegrees(MaxAngularVelocity);
	// 애니메이션으로 대체
}

void AEnemy_DropBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnMovement();
}

void AEnemy_DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_DropBase::OnDestinatingGround()
{
	SetActorTickEnabled(false);
	
}

bool AEnemy_DropBase::BeginFocus()
{
	TArray<AActor*> Actors;
	InteractionTrigger->GetOverlappingActors(Actors, AArcanePunkCharacter::StaticClass());

	if(Actors.IsEmpty()) return false;

	auto PlayerCharacter = Cast<AArcanePunkCharacter>(Actors.Top()); if(!PlayerCharacter) return false;
	OnRooting(PlayerCharacter);	

	return true;
}

void AEnemy_DropBase::SpawnMovement()
{
	// float Impulse_X = FMath::RandRange(-SpawnImpulse.X, SpawnImpulse.X);
	// float Impulse_Y = FMath::RandRange(-SpawnImpulse.Y,  SpawnImpulse.Y);
	// float Impulse_Z = FMath::RandRange(SpawnImpulse.Z*0.8,  SpawnImpulse.Z*1.2);

	// DropMesh->AddImpulse(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
	// DropMesh->AddAngularImpulseInDegrees(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
}

void AEnemy_DropBase::InitializePickup(AActor *DamageCauser, const int32 Quantity, bool RootingImmediate)
{
	if (!ItemDataTable || DesiredItemID.IsNone()) return;
	
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

	ItemReference = NewObject<UAPItemBase>(this, UAPItemBase::StaticClass());

	ItemReference->ID = ItemData->ID;
	ItemReference->ItemType = ItemData->ItemType;
	ItemReference->ItemQuality = ItemData->ItemQuality;
	ItemReference->ItemNumericData = ItemData->ItemNumericData;
	ItemReference->ItemTextData = ItemData->ItemTextData;
	ItemReference->ItemAssetData = ItemData->ItemAssetData;
	ItemReference->ItemStatistics = ItemData->ItemStatistics;

	DropMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

	Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);
	
	if(RootingImmediate)
	{
		auto PlayerCharacter = Cast<AArcanePunkCharacter>(DamageCauser); if(!PlayerCharacter) return;

		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy_DropBase::OnRooting, PlayerCharacter);
		GetWorld()->GetTimerManager().SetTimer(RootingTimerHandle, TimerDelegate, RootingTime, false);
	}
}

void AEnemy_DropBase::OnRooting(AArcanePunkCharacter* PlayerCharacter)
{
	PlayerCharacter->GetInventory()->HandleAddItem(ItemReference);

	auto SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI) return;
    float Volume = 1.0f; Volume *= SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;

	UGameplayStatics::SpawnSound2D(GetWorld(), DropSound, Volume);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DropEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}
