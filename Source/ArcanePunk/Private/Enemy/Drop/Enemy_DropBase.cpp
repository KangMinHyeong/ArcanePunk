// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemy_DropBase::AEnemy_DropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("InteractTrigger"));
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));

	SetRootComponent(DropMesh);
	InteractTrigger->SetupAttachment(DropMesh);

	DropMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("DropMovement"));
	DropMovement->MaxSpeed = DropSpeed;
	DropMovement->InitialSpeed = DropSpeed;

	DropMesh->SetSimulatePhysics(true);
	DropMesh->BodyInstance.bLockXRotation = true;
	DropMesh->BodyInstance.bLockYRotation = true;
	DropMesh->SetPhysicsMaxAngularVelocityInDegrees(MaxAngularVelocity);
	// 애니메이션으로 대체
}

// Called when the game starts or when spawned
void AEnemy_DropBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	SpawnMovement();
}

// Called every frame
void AEnemy_DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_DropBase::SpawnMovement()
{
	float Impulse_X = FMath::RandRange(-SpawnImpulse.X, SpawnImpulse.X);
	float Impulse_Y = FMath::RandRange(-SpawnImpulse.Y,  SpawnImpulse.Y);
	float Impulse_Z = FMath::RandRange(SpawnImpulse.Z*0.8,  SpawnImpulse.Z*1.2);

	DropMesh->AddImpulse(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
	DropMesh->AddAngularImpulseInDegrees(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
	DropMesh->AddImpulse(-GetActorUpVector()*GravityAccelerate, TEXT("DropMesh"), true);
}

void AEnemy_DropBase::InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UAPItemBase>(this, BaseClass);

		// Itema Data Register

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;

		DropMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);
	}
}