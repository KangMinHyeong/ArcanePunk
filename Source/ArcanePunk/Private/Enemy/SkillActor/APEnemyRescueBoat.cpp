

#include "Enemy/SkillActor/APEnemyRescueBoat.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy_CharacterBase.h"

AAPEnemyRescueBoat::AAPEnemyRescueBoat()
{
	PrimaryActorTick.bCanEverTick = true;

	RescueRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RescueRoot"));
	RescueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RescueMesh"));

	SetRootComponent(RescueRoot);
	RescueMesh->SetupAttachment(RescueRoot);
}

void AAPEnemyRescueBoat::BeginPlay()
{
	Super::BeginPlay();
}

void AAPEnemyRescueBoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bRescue)
	{
		FVector Current = GetActorLocation();
		Current = FMath::VInterpConstantTo(Current, RescuePoint, DeltaTime, RescueSpeed);
		SetActorLocation(Current);

		if((Current - RescuePoint).Size() < 1.0f) OnRescuing();
	}
}

void AAPEnemyRescueBoat::SetRescuePoint(AActor* Actor)
{
	RescueActor = Actor;
	RescuePoint = RescueActor->GetActorLocation();
	bRescue = true;

	FRotator Rotation = FRotationMatrix::MakeFromX(GetActorLocation() - RescuePoint).Rotator();
	SetActorRotation(Rotation);

	SetActorTickEnabled(true);
}

void AAPEnemyRescueBoat::OnRescuing()
{
	auto Enemy = Cast<AEnemy_CharacterBase>(RescueActor.Get()); if(!Enemy) return;
	Enemy->RescueEnemy();
	bRescue = false;
}
