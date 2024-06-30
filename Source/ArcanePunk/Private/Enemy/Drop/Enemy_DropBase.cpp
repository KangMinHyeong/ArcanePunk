// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy_DropBase::AEnemy_DropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("InteractTrigger"));
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	DropTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropTrailEffect"));
	GroundTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GroundTrigger"));

	SetRootComponent(DropMesh);
	InteractTrigger->SetupAttachment(DropMesh);
	DropTrailEffect->SetupAttachment(DropMesh);
	GroundTrigger->SetupAttachment(DropMesh);

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

	DropTrailEffect->DeactivateImmediate();
	SpawnMovement();
	GroundTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_DropBase::OnOverlap);
}

// Called every frame
void AEnemy_DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentAccelerate = FMath::FInterpConstantTo(CurrentAccelerate, GravityAccelerate, DeltaTime, AccelerateSpeed);
	DropMesh->AddForce(-GetActorUpVector()*CurrentAccelerate, TEXT("DropMesh"), true);

	if( GravityAccelerate - CurrentAccelerate <= KINDA_SMALL_NUMBER) OnDestinatingGround();
}

void AEnemy_DropBase::OnDestinatingGround()
{
	SetActorTickEnabled(false);
	
}

void AEnemy_DropBase::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	DropTrailEffect->DeactivateImmediate();
}

void AEnemy_DropBase::SpawnMovement()
{
	float Impulse_X = FMath::RandRange(-SpawnImpulse.X, SpawnImpulse.X);
	float Impulse_Y = FMath::RandRange(-SpawnImpulse.Y,  SpawnImpulse.Y);
	float Impulse_Z = FMath::RandRange(SpawnImpulse.Z*0.8,  SpawnImpulse.Z*1.2);

	DropMesh->AddImpulse(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
	DropMesh->AddAngularImpulseInDegrees(FVector(Impulse_X, Impulse_Y, Impulse_Z), TEXT("DropMesh"), true);
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