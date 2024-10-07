
#include "Skill/WindRush.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AWindRush::AWindRush()
{
    WindRushTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WindRushTrigger"));

    SetRootComponent(WindRushTrigger);
}


void AWindRush::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AWindRush::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!OwnerCharacter.IsValid()) return;
    FVector Current = OwnerCharacter->GetActorLocation();
    Current = FMath::VInterpConstantTo(Current, TargetLocation, DeltaTime, RushSpeed);
    OwnerCharacter->SetActorLocation(Current);
    if(abs(Current.X - TargetLocation.X) < 0.1f && abs(Current.Y - TargetLocation.Y) < 0.1f)
    {
        DestroySKill();
    }
}

void AWindRush::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetOwner()->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
        
        auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
        if(Enemy) Enemy->GetCrowdControlComp()->BleedingState(OwnerCharacter.Get(), BleedingDamage, StateTime, BleedingRate);
	}
}

void AWindRush::DestroySKill() 
{
    DeActivate_Ult();
    SetPlayerCollisionEnable(true);
    // RushEffectComp->DestroyComponent();
    Super::DestroySKill();
}

void AWindRush::SetWindRushWidth(float Width)
{
    WindRushTrigger->SetBoxExtent(FVector(5.0f, Width, Width), true);
}

void AWindRush::SetTargetLocation(const FVector & Location, float Dist)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    TargetLocation = (Location/Location.Size()) * Dist + OwnerCharacter->GetActorLocation();
}

void AWindRush::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    WindRushTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWindRush::OnOverlap);
    WindRushTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    SetPlayerCollisionEnable(false);
    RushEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(WindRushEffect, OwnerCharacter->GetMesh(), TEXT("WindRushEffect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
}

