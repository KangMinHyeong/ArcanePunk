
#include "Skill/ArcaneField.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"

AArcaneField::AArcaneField()
{
    FieldTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("FieldTrigger"));
    FieldDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("FieldDecal"));

    SetRootComponent(FieldTrigger);
    FieldDecal->SetupAttachment(FieldTrigger);

    SkillCategory = ESkillCategory::InvisibleCollider;
}

void AArcaneField::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    SetActorTickEnabled(true);
    
    HitPointComp->SetSlowCoefficient(SlowCoefficient);
    SetFieldDebuff();
    GetWorld()->GetTimerManager().SetTimer(FieldTimerHandle, this, &AArcaneField::SetFieldDebuff, DebuffRateTime, true);
    GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AArcaneField::SetFieldDamage, DamageRateTime, true);
}

void AArcaneField::SetFieldDebuff()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {               
            HitPointComp->SetCrowdControl(Actor, ECharacterState::Slow, DebuffRateTime-0.01f);
        }
    }    
}

void AArcaneField::SetFieldDamage()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {               
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); bool AlreadyHitCheck = Enemy->IsHitting();
            UGameplayStatics::ApplyDamage(Actor, OwnerCharacter->GetFinalATK() * DamageCoefficient, OwnerCharacter->GetInstigatorController(), this, UDamageType::StaticClass());
            if(!AlreadyHitCheck) Enemy->SetHitting(false);            
        }
    }    
}

void AArcaneField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FollowCharacter(DeltaTime);
}

void AArcaneField::FollowCharacter(float DeltaTime)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(!OwnerCharacter.IsValid()) return;
    SetActorLocation(FMath::VInterpTo(GetActorLocation(), FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, GetActorLocation().Z) , DeltaTime, FollowSpeed));
}

void AArcaneField::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);

    // FieldTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneField::SetFieldDebuff);
}

void AArcaneField::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_11);

    Super::DestroySKill();
}
