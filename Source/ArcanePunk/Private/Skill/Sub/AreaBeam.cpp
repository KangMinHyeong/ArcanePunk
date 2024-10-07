
#include "Skill/Sub/AreaBeam.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"

AAreaBeam::AAreaBeam()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaBeamRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AreaBeamRoot"));
	SetRootComponent(AreaBeamRoot);

	AreaStartEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AreaStartEffect"));
	AreaStartEffect->SetupAttachment(AreaBeamRoot);
}

void AAreaBeam::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AAreaBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAreaBeam::DestroySKill()
{
    Super::DestroySKill();
}

void AAreaBeam::SetBeamRotation(FVector TargetVector)
{
	BeamEnd = TargetVector;
	FVector Loc = BeamEnd - GetActorLocation(); Loc.Z = 0.0f; 
	
	SetActorRotation(FRotationMatrix::MakeFromX(Loc).Rotator());
}

void AAreaBeam::OnBeamAttack()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	AreaStartEffect->DeactivateImmediate();
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AreaBeamEffect, GetActorLocation(), GetActorRotation());
	NC->SetVariableFloat(TEXT("Time"),  DestroyTime);

	float Width = AreaWidth / InitWidth;
    float Length = AreaLength / InitLength;
	NC->SetVariableFloat(TEXT("Width"),  Width);
    NC->SetVariableFloat(TEXT("Length"),  Length);
    NC->SetVariableVec2(TEXT("Size2D"),  FVector2D(Width, Length));
    NC->SetVariableVec3(TEXT("Size3D"),  FVector(Width, Length, 1.0f));
    NC->SetVariableVec3(TEXT("AddVector"),  FVector((AreaLength - InitLength) * 0.5f, 0.0f, 0.0f));

	// Attack
	SetAreaDamage();
}

void AAreaBeam::SetAreaDamage()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), BeamEnd, AreaWidth, DamageCoefficient, 1, bStun, StateTime);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAreaBeam::SetAreaDamage, AreaDamageRate, false);
}

void AAreaBeam::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

	AreaStartEffect->SetVariableFloat(TEXT("Time"),  BeamDelay);
	AreaStartEffect->Activate();
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAreaBeam::OnBeamAttack, BeamDelay, false);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAreaBeam::DestroySKill, DestroyTime, false);
}
