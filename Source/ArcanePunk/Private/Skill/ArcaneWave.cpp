
#include "Skill/ArcaneWave.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneWave::AArcaneWave()
{
    WaveOutSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WaveOutSphere"));

    SetRootComponent(WaveOutSphere);
}

void AArcaneWave::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneWave::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!OwnerCharacter.IsValid()) return;

    SetInAndOutRadius(DeltaTime);
}

void AArcaneWave::DestroySKill()
{
    Super::DestroySKill();
}

void AArcaneWave::SetInAndOutRadius(float DeltaTime)
{
    OutRadius = FMath::FInterpConstantTo(OutRadius, MaxDist, DeltaTime, WaveSpeed);

    WaveOutSphere->SetSphereRadius(OutRadius, false);

    if(abs(OutRadius - MaxDist) <= KINDA_SMALL_NUMBER) DestroySKill();
}

void AArcaneWave::OnOverlaping_OutSphere(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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
		HitDelay(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitNumbers, HitDelayTime, true);
	}
}

void AArcaneWave::SetWaveSize()
{
    float CurrentSize = MaxDist / WaveOutSphere->GetScaledSphereRadius();
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WaveEffect, GetActorLocation(), GetActorRotation());
    NC->SetNiagaraVariableFloat(TEXT("Size"), CurrentSize);
    NC->SetNiagaraVariableFloat(TEXT("Speed"), WaveSpeed / MaxDist);

    OutRadius = 0.0f;
    WaveOutSphere->SetSphereRadius(OutRadius, true);
}

void AArcaneWave::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());  if(!OwnerCharacter.IsValid()) return;
    
    WaveOutSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WaveOutSphere->OnComponentBeginOverlap.AddDynamic(this, &AArcaneWave::OnOverlaping_OutSphere);

    SetWaveSize();
    SetActorTickEnabled(true);
}

