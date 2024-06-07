
#include "Skill/CarpetBoom.h"

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

ACarpetBoom::ACarpetBoom()
{
    CarpetBoomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CarpetBoomTrigger"));
    BoomEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BoomEffectComp")); 

    SetRootComponent(CarpetBoomTrigger);
    BoomEffectComp->SetupAttachment(CarpetBoomTrigger);
}

void ACarpetBoom::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ACarpetBoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Current = GetActorLocation();
    Current = FMath::VInterpConstantTo(Current, BoomTargetLocation, DeltaTime, BoomSpeed);
    SetActorLocation(Current);

    if(abs(Current.X - BoomTargetLocation.X) < 0.1f && abs(Current.Y - BoomTargetLocation.Y) < 0.1f)
    {
        DestroySKill();
    }
}

void ACarpetBoom::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetOwner()->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

    if(!OwnerCharacter.IsValid() && OtherActor && OtherActor != this) return;

	if(OtherActor != MyOwner)
	{
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
	}
    else
    {
        auto PDD = OwnerCharacter->GetPlayerStatus(); float OriginHP = PDD.PlayerDynamicData.HP;
        
        float HP = PDD.PlayerDynamicData.HP + (PDD.PlayerDynamicData.MaxHP - PDD.PlayerDynamicData.HP) * HPRecoveryPercent; 
        PDD.PlayerDynamicData.HP = FMath::Min(PDD.PlayerDynamicData.MaxHP, HP);

        OwnerCharacter->SetDefaultHP(PDD.PlayerDynamicData.HP); 
        OwnerCharacter->GetAPHUD()->OnUpdateHPBar.Broadcast(OriginHP);
    }
}

void ACarpetBoom::DestroySKill() 
{
    // LavaEffectComp->DestroyComponent();
    Super::DestroySKill();
}

void ACarpetBoom::SetCarpetBoomWidth(float Width)
{
    CollisionWidth = Width;
    CarpetBoomTrigger->SetBoxExtent(FVector(5.0f, Width, Width), true);
}

void ACarpetBoom::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    LavaEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LavaEffect, GetActorLocation(), GetActorRotation());
    float WidthSize = CollisionWidth * 0.01f; float SpeedScale = CollisionLength * 0.001f;
    LavaEffectComp->SetNiagaraVariableFloat(TEXT("Size"),  WidthSize);
    // LavaEffectComp->SetNiagaraVariableFloat(TEXT("SpeedScale"), SpeedScale);
    LavaEffectComp->SetNiagaraVariableFloat(TEXT("SpawnRate"),  (DefaultSpawnRate * SpeedScale) / WidthSize);
    LavaEffectComp->SetNiagaraVariableFloat(TEXT("LoopDuration"), CollisionLength / BoomSpeed);

    // FVector BoomScale = BoomEffectComp->GetComponentScale();
    BoomEffectComp->SetNiagaraVariableFloat(TEXT("Size"),  WidthSize);
    

    UE_LOG(LogTemp, Display, TEXT("size %f, speed : %f, spawn : %f"), WidthSize, SpeedScale, (DefaultSpawnRate * BoomSpeed) / WidthSize );

    CarpetBoomTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACarpetBoom::OnOverlap);
    CarpetBoomTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    CarpetBoomTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
    
    SetActorTickEnabled(true);
    // RushEffectComp = 
}