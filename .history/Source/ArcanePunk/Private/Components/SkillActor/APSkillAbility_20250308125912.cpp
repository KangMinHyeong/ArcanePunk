
#include "Components/SkillActor/APSkillAbility.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/ShapeComponent.h"

UAPSkillAbility::UAPSkillAbility()
{
	PrimaryComponentTick.bCanEverTick = true;

	// OwnerProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("OwnerProjectileMove"));
	
	// OwnerProjectileMove->InitialSpeed = 0.0f;
	// OwnerProjectileMove->MaxSpeed = 2000.0f;
	// OwnerProjectileMove->ProjectileGravityScale = 0.0f;
	// OwnerProjectileMove->bIsSliding = true;
}

void UAPSkillAbility::BeginPlay()
{
	Super::BeginPlay();
	// OwnerProjectileMove->ProjectileGravityScale = 0.0f;
	SetComponentTickEnabled(false);
}

void UAPSkillAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// switch (SkillCategory)
	// {
	// 	case ESkillCategory::Projecitle:
	// 	ProjectileHoming(DeltaTime);
	// 	break;

	// 	case ESkillCategory::Throw:
	// 	AddImpulse();
	// 	break;

	// 	case ESkillCategory::Laser:
	// 	break;
	// }
	CurrentScale = FMath::VInterpConstantTo(CurrentScale, TargetScale, DeltaTime, GigantCoefficient);
	GetOwner()->SetActorScale3D(CurrentScale);
}

void UAPSkillAbility::Coefficient_Add(float & Current, float Add, uint16 NestingNum)
{
	Current = Current + Add * NestingNum;
	// for(uint16 i = 0; i<NestingNum; i++) Current = Current + Add;
}

void UAPSkillAbility::Coefficient_Add(int32 & Current, float Add, uint16 NestingNum)
{
	Current = Current + static_cast<int32>(Add) * NestingNum;
	// for(uint16 i = 0; i<NestingNum; i++) Current = Current + Add;
}

float UAPSkillAbility::Coefficient_Add_Return(float Current, float Add, uint16 NestingNum)
{
	return Current + Add * NestingNum;
	// for(uint16 i = 0; i<NestingNum; i++) Current = Current + Add;
}

int32 UAPSkillAbility::Coefficient_Add_Return(int32 Current, float Add, uint16 NestingNum)
{
	return Current + static_cast<int32>(Add) * NestingNum;
	// for(uint16 i = 0; i<NestingNum; i++) Current = Current + Add;
}

void UAPSkillAbility::Coefficient_Multiple(float &Current, float Coefficient, uint16 NestingNum)
{
	Current = Current * FMath::Pow(Coefficient, NestingNum);
}

float UAPSkillAbility::Coefficient_Multiple_Return(float Current, float Coefficient, uint16 NestingNum)
{
	return Current * FMath::Pow(Coefficient, NestingNum);
}

void UAPSkillAbility::Coefficient_AddMultiple(float &Current, float Coefficient, uint16 NestingNum)
{
	Current = Current + Current*(Coefficient * NestingNum);
}

void UAPSkillAbility::Coefficient_AddMultiple(int32 &Current, float Coefficient, uint16 NestingNum)
{
	Current = Current + Current*(static_cast<uint8>(Coefficient) * NestingNum);
}

void UAPSkillAbility::Coefficient_SubtractMultiple(float &Current, float Coefficient, uint16 NestingNum)
{
	Current = FMath::Max(Current - Current*(Coefficient * NestingNum), 0.0f);
}

void UAPSkillAbility::GigantGradually(float Coefficient, uint16 NestingNum)
{
	GigantCoefficient = GigantCoefficient * FMath::Pow(Coefficient, NestingNum);
	CurrentScale = GetOwner()->GetActorScale3D(); TargetScale = CurrentScale * 15.0f;
	SetComponentTickEnabled(true);
}

float UAPSkillAbility::SizeUp_Multiple(float Coefficient, uint16 NestingNum)
{
	return FMath::Pow(Coefficient, NestingNum);;
}

// void UAPSkillAbility::SetOwnerScale()
// {
// 	GetOwner()->SetActorScale3D(GetOwner()->GetActorScale3D() * 2.0f);
// }

// void UAPSkillAbility::SetHoming()
// {
// 	switch (SkillCategory)
// 	{
// 		case ESkillCategory::Projecitle:
// 		GetWorld()->GetTimerManager().SetTimer(AccelerateTimerHandle, this, &UAPSkillAbility::Accelrating, HomingTime, false);
// 		SetProjectileMove();
// 		break;

// 		case ESkillCategory::Throw:
// 		GetWorld()->GetTimerManager().SetTimer(AccelerateTimerHandle, this, &UAPSkillAbility::Accelrating, HomingTime, false);
// 		break;

// 		case ESkillCategory::Laser:
// 		break;
// 	}
// }

// void UAPSkillAbility::Accelrating()
// {
// 	SetComponentTickEnabled(true);
// 	SetTraceActor();
// 	GetWorld()->GetTimerManager().ClearTimer(AccelerateTimerHandle);
// }

// void UAPSkillAbility::SetProjectileMove()
// {
// 	if(GetOwner()->GetComponentByClass<UProjectileMovementComponent>()) OriginProjectileMoveComp = GetOwner()->GetComponentByClass<UProjectileMovementComponent>();
	
// 	if(GetOwner()->GetComponentByClass<UShapeComponent>()) 
// 	{
// 		if(!GetOwner()->GetComponentByClass<UShapeComponent>()->OnComponentBeginOverlap.IsBound()) GetOwner()->GetComponentByClass<UShapeComponent>()->OnComponentBeginOverlap.AddDynamic(this, &UAPSkillAbility::HomingEnd);
// 	}
// }

// void UAPSkillAbility::SetTraceActor(AActor* OverlapActor)
// {
// 	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()->GetOwner()); if(!OwnerCharacter) return;

// 	if(Actors.IsEmpty())
// 	{
// 		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
// 	}
// 	if(Actors.IsEmpty()) return;
	
// 	if(Actors.Num() >= 2)
// 	{
// 		if(Init)
// 		{
// 			Actors.Sort([OwnerCharacter](AActor& A, AActor& B) 
// 			{
// 				return (A.GetActorLocation() - OwnerCharacter->GetHomingPoint()).Size() > (B.GetActorLocation() - OwnerCharacter->GetHomingPoint()).Size();
// 			});
// 			Init = false;
// 		}
// 		else
// 		{
// 			Actors.Sort([this](AActor& A, AActor& B) 
// 			{
// 				return (A.GetActorLocation() - GetOwner()->GetActorLocation()).Size() > (B.GetActorLocation() - GetOwner()->GetActorLocation()).Size();
// 			});
// 		}
// 	}

// 	if(Actors.Top() == OverlapActor) Actors.Pop();
// 	if(Actors.IsEmpty())
// 	{
// 		TraceActor = nullptr;
// 	}
// 	else
// 	{
// 		float MaxHomingDistance = 10000.0f;
// 		float Dist = (Actors.Top()->GetActorLocation() - GetOwner()->GetActorLocation()).Size();

// 		if(Dist < MaxHomingDistance)
// 		{
// 			TraceActor = Actors.Top();
// 			Actors.Pop();
// 		}
// 	}
	
// 	if(SkillCategory == ESkillCategory::Projecitle) SetProjectileValue();
// }

// void UAPSkillAbility::SetProjectileValue()
// {
// 	if(TraceActor.IsValid())
// 	{		
// 		OwnerProjectileMove->bInterpMovement = true;
// 		OwnerProjectileMove->bInterpRotation = true;
// 		OwnerProjectileMove->bRotationFollowsVelocity = true;
// 		OwnerProjectileMove->bIsHomingProjectile = true;
// 		OwnerProjectileMove->HomingTargetComponent = TraceActor->GetRootComponent();

// 	}
// 	else
// 	{
// 		OwnerProjectileMove->bRotationFollowsVelocity = false;
// 		OwnerProjectileMove->bIsHomingProjectile = false;
// 		OwnerProjectileMove->HomingTargetComponent = nullptr;
// 	}
// }

// void UAPSkillAbility::HomingEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
// {
// 	if(OtherActor->ActorHasTag(TEXT("Enemy")))
// 	{
// 		GetWorld()->GetTimerManager().ClearTimer(AccelerateTimerHandle);
// 		SetTraceActor(OtherActor);
// 	}
// }

// void UAPSkillAbility::ProjectileHoming(float DeltaTime)
// {
// 	if(OriginProjectileMoveComp) 
// 	{
// 		if(!TraceActor.IsValid())
// 		{
// 			OriginProjectileMoveComp->MaxSpeed = FMath::FInterpTo(OriginProjectileMoveComp->MaxSpeed, 2250.0f, DeltaTime, TraceSpeed*5);
// 			OwnerProjectileMove->HomingAccelerationMagnitude = FMath::FInterpTo(OwnerProjectileMove->HomingAccelerationMagnitude, 0.1f, DeltaTime, TraceSpeed);
// 		}
// 		else
// 		{
// 			OriginProjectileMoveComp->MaxSpeed = FMath::FInterpConstantTo(OriginProjectileMoveComp->MaxSpeed, 0.001f, DeltaTime, TraceSpeed*3.0f); OriginProjectileMoveComp->InitialSpeed = 0.001f;
// 			OwnerProjectileMove->HomingAccelerationMagnitude = FMath::FInterpConstantTo(OwnerProjectileMove->HomingAccelerationMagnitude, OwnerProjectileMove->MaxSpeed, DeltaTime, TraceSpeed*3.8f);
// 			OwnerProjectileMove->MaxSpeed = FMath::FInterpConstantTo(OwnerProjectileMove->MaxSpeed, 5500.0f, DeltaTime, TraceSpeed*2.5f);
// 		}
// 	}
// }

// void UAPSkillAbility::AddImpulse()
// {
// 	if(!TraceActor.IsValid()) return;;
	
// 	FVector HomingVector = (TraceActor->GetActorLocation() - GetOwner()->GetActorLocation());
// 	HomingVector = (HomingVector /HomingVector.Size() )* TraceSpeed;
// 	GetOwner()->GetComponentByClass<UStaticMeshComponent>()->AddForce(HomingVector, TEXT("HomingVector"), false);
// }

// void UAPSkillAbility::SetStun()
// {
// 	auto SkillActor = Cast<AAPSkillActorBase>(GetOwner()); if(!SkillActor) return;
	
// 	SkillActor->SetbStun(true);
// }


