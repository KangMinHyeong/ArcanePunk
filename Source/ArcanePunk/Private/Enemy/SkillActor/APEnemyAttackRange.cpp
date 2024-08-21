
#include "Enemy/SkillActor/APEnemyAttackRange.h"

#include "Components/DecalComponent.h"

AAPEnemyAttackRange::AAPEnemyAttackRange()
{
	PrimaryActorTick.bCanEverTick = true;

	RangeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RangeRoot"));
	BaseDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("BaseDecal"));
	PatternDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("PatternDecal"));

	SetRootComponent(RangeRoot);
	BaseDecal->SetupAttachment(RangeRoot);
	PatternDecal->SetupAttachment(RangeRoot);
}

void AAPEnemyAttackRange::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPEnemyAttackRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(abs(CurrentMove - 1.0f) <= KINDA_SMALL_NUMBER)
	{
		Destroy();
	}
	else
	{
		CurrentMove = FMath::FInterpConstantTo(CurrentMove, 1.0f, DeltaTime, MoveSpeed);

		PatternDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(ScalarName, CurrentMove);
	}
}

void AAPEnemyAttackRange::SetDecalSize(float X, float Y, float AttackRangeTime, bool bSquare)
{
	if(bSquare)
	{
		SetActorScale3D(FVector(1.0f, X / 200.0f, Y / 400.0f));
		FVector Loc = (GetActorForwardVector() * Y * 0.5f) + GetActorLocation();
		BaseDecal->SetWorldLocation(Loc);
		PatternDecal->SetWorldLocation(Loc);
		
		ScalarName = TEXT("Move_X");
	}
	else
	{
		SetActorScale3D(FVector(1.0f, X / 200.0f, Y / 200.0f));
		
		ScalarName = TEXT("Dissolve");
	}	

	MoveSpeed = 2.0f / AttackRangeTime;
	
	PatternDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(ScalarName, CurrentMove);
}
