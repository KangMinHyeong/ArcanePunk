// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	DropTrailEffect->DeactivateImmediate();
	SpawnMovement();
}

void AEnemy_DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CurrentAccelerate = FMath::FInterpConstantTo(CurrentAccelerate, GravityAccelerate, DeltaTime, AccelerateSpeed);
	// DropMesh->AddForce(-GetActorUpVector()*CurrentAccelerate, TEXT("DropMesh"), true);

	// if( GravityAccelerate - CurrentAccelerate <= KINDA_SMALL_NUMBER) OnDestinatingGround();
}

void AEnemy_DropBase::OnDestinatingGround()
{
	SetActorTickEnabled(false);
	
}

void AEnemy_DropBase::BeginFocus()
{
	TArray<AActor*> Actors;
	InteractionTrigger->GetOverlappingActors(Actors, AArcanePunkCharacter::StaticClass());

	if(Actors.IsEmpty()) return;

	auto PlayerCharacter = Cast<AArcanePunkCharacter>(Actors.Top()); if(!PlayerCharacter) return;
	
	// if(DropItems->ID == "Gold") {DropItems->SetQuantity(CheckGoldAmount());}
	PlayerCharacter->GetInventory()->HandleAddItem(ItemReference);

	Destroy();
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
	
		// if(DropItems->ID == "Gold") {DropItems->SetQuantity(CheckGoldAmount());}
		PlayerCharacter->GetInventory()->HandleAddItem(ItemReference);

		UGameplayStatics::SpawnSound2D(GetWorld(), DropSound, DropSoundVolume);

		Destroy();
	}
}