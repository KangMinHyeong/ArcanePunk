
#include "Components/SkillActor/APSkillType.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"

UAPSkillType::UAPSkillType()
{
	PrimaryComponentTick.bCanEverTick = true;

	OwnerProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("OwnerProjectileMoveComp"));
	
	OwnerProjectileMoveComp->InitialSpeed = 0.0f;
	OwnerProjectileMoveComp->MaxSpeed = 5000.0f;
}

void UAPSkillType::BeginPlay()
{
	Super::BeginPlay();
	OwnerProjectileMoveComp->ProjectileGravityScale = 0.0f;
}

void UAPSkillType::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!TraceActor)
	{
		if(OriginProjectileMoveComp) {OriginProjectileMoveComp->MaxSpeed = FMath::FInterpTo(OriginProjectileMoveComp->MaxSpeed, 2250.0f, DeltaTime, TraceSpeed*5);}
		OwnerProjectileMoveComp->HomingAccelerationMagnitude = FMath::FInterpTo(OwnerProjectileMoveComp->HomingAccelerationMagnitude, 0.1f, DeltaTime, TraceSpeed);
	}
	else
	{
		if(OriginProjectileMoveComp) {OriginProjectileMoveComp->MaxSpeed = FMath::FInterpConstantTo(OriginProjectileMoveComp->MaxSpeed, 0.001f, DeltaTime, TraceSpeed*2.1f); OriginProjectileMoveComp->InitialSpeed = 0.001f;}
		OwnerProjectileMoveComp->HomingAccelerationMagnitude = FMath::FInterpConstantTo(OwnerProjectileMoveComp->HomingAccelerationMagnitude, OwnerProjectileMoveComp->MaxSpeed, DeltaTime, TraceSpeed*2.5f);
		OwnerProjectileMoveComp->MaxSpeed = FMath::FInterpConstantTo(OwnerProjectileMoveComp->MaxSpeed, 7500.0f, DeltaTime, TraceSpeed*2.5f);
	}
}

void UAPSkillType::SetSkillType(uint8 SkillType, bool& bStun, UPrimitiveComponent* TriggerComp, UProjectileMovementComponent* ProjectileMovementComponent)
{
	switch (SkillType)
	{
		case 0:
		break;

		case 1:
		SetOwnerScale();
		break;

		case 2:
		SetProjectileMove(TriggerComp, ProjectileMovementComponent);
		break;

		case 3:
		bStun = true;
		break;
	}
}

void UAPSkillType::SetOwnerScale()
{
	GetOwner()->SetActorScale3D(GetOwner()->GetActorScale3D() * 2.0f);
}

void UAPSkillType::SetProjectileMove(UPrimitiveComponent* TriggerComp, UProjectileMovementComponent *ProjectileMovementComponent)
{
	if(ProjectileMovementComponent) 
	{
		OriginProjectileMoveComp = ProjectileMovementComponent;

		GetWorld()->GetTimerManager().SetTimer(AccelerateTimerHandle, this, &UAPSkillType::Accelrating, 0.15f, false);

		if(!bTriggerOn)
		{ 
			TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &UAPSkillType::HomingEnd); bTriggerOn = true;
		}
	}
}

void UAPSkillType::SetOwnerHoming(AActor* OverlapActor)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()->GetOwner()); if(!OwnerCharacter) return;

	if(Actors.IsEmpty())
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
	}
	if(Actors.IsEmpty() || Actors.Num() < 2) return;
	
	if(Init)
	{
		Actors.Sort([OwnerCharacter](AActor& A, AActor& B) 
		{
			return (A.GetActorLocation() - OwnerCharacter->GetHomingPoint()).Size() > (B.GetActorLocation() - OwnerCharacter->GetHomingPoint()).Size();
		});
		Init = false;
	}
	else
	{
		Actors.Sort([this](AActor& A, AActor& B) 
		{
			return (A.GetActorLocation() - GetOwner()->GetActorLocation()).Size() > (B.GetActorLocation() - GetOwner()->GetActorLocation()).Size();
		});
	}
	

	if(Actors.Top() == OverlapActor) Actors.Pop();
	if(Actors.IsEmpty())
	{
		TraceActor = nullptr;
	}
	else
	{
		float MaxHomingDistance = 10000.0f;
		float Dist = (Actors.Top()->GetActorLocation() - GetOwner()->GetActorLocation()).Size();

		if(Dist < MaxHomingDistance)
		{
			TraceActor = Actors.Top();
			Actors.Pop();
		}
	}
	
	if(TraceActor)
	{		
		OwnerProjectileMoveComp->bInterpMovement = true;
		OwnerProjectileMoveComp->bInterpRotation = true;
		OwnerProjectileMoveComp->bRotationFollowsVelocity = true;
		OwnerProjectileMoveComp->bIsHomingProjectile = true;
		OwnerProjectileMoveComp->HomingTargetComponent = TraceActor->GetRootComponent();
	}
	else
	{
		OwnerProjectileMoveComp->bRotationFollowsVelocity = false;
		OwnerProjectileMoveComp->bIsHomingProjectile = false;
		OwnerProjectileMoveComp->HomingTargetComponent = nullptr;
	}
}

void UAPSkillType::Accelrating()
{
	SetOwnerHoming();
	GetWorld()->GetTimerManager().ClearTimer(AccelerateTimerHandle);
}

void UAPSkillType::HomingEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		GetWorld()->GetTimerManager().ClearTimer(AccelerateTimerHandle);
		SetOwnerHoming(OtherActor);
	}	
}
