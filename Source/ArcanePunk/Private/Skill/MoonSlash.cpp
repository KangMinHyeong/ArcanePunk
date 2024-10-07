
#include "Skill/MoonSlash.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Character/APAttackComponent.h"

AMoonSlash::AMoonSlash()
{
    MoonSlashRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MoonSlashRoot"));
    MoonSlashTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MoonSlashTrigger"));

    SetRootComponent(MoonSlashRoot);
    MoonSlashTrigger->SetupAttachment(MoonSlashRoot);

}

void AMoonSlash::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AMoonSlash::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurrentRot = FMath::RInterpConstantTo(CurrentRot, TargetRot, DeltaTime, MoonSlashSpeed);
    SetActorRotation(CurrentRot);

}

void AMoonSlash::DestroySKill()
{
    Super::DestroySKill();
}

void AMoonSlash::ApplyMoonSlashSetting()
{
    MoonSlashTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MoonSlashTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMoonSlash::OnSlashDamage);

    MoonSlashTrigger->SetBoxExtent(FVector(MoonSlashWidth, 10.5f, MoonSlashWidth));
    MoonSlashTrigger->SetRelativeLocation(FVector(MoonSlashWidth, 0.0f, 0.0f));
    SetActorRotation( GetActorRotation() + FRotator(0.0f, MoonSlashAngle * 0.5f * 360.0f, 0.0f));

    CurrentRot = GetActorRotation();
    TargetRot = GetActorRotation() - FRotator(0.0f, MoonSlashAngle * 360.0f, 0.0f);

    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MoonSlashEffect, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation() + PlusEffectRot);
    NC->SetVariableFloat(TEXT("Size"),  MoonSlashWidth * 0.01f);
    NC->SetVariableFloat(TEXT("Speed"),  MoonSlashSpeed * 0.001f);

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMoonSlash::DestroySKill, (MoonSlashAngle * 360.0f) / MoonSlashSpeed, false);
}

void AMoonSlash::OnSlashDamage(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{			
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
        OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
	}
}

void AMoonSlash::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

    ApplyMoonSlashSetting();

    SetActorTickEnabled(true);
}
