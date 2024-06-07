
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

	SlowPercent = 30;
}

void AArcaneField::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);   
}

void AArcaneField::SetFieldDebuff()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {              
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
            HitPointComp->SetCrowdControl(Actor, ECharacterState::Slow, DebuffRateTime-0.001f);
            if(bWeak) Enemy->GetCrowdControlComp()->WeakState(WeakCoefficient, DebuffRateTime-0.001f);    
        }
    }    
}

void AArcaneField::SetFieldDamage()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {               
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); bool AlreadyHitCheck = Enemy->IsHitting();
            if(BuffType == EBuffType::Damage) {Enemy->SetDamageMultiple(DamageBuff, DamageRateTime - 0.001f);} FHitResult HitResult;
            
            float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Enemy, FieldDamage * DamageCoefficient, HitResult, false);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(Enemy, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
            // UGameplayStatics::ApplyDamage(Actor,  * OwnerCharacter->CriticalCalculate(), OwnerCharacter->GetInstigatorController(), this, UDamageType::StaticClass());
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
    if(!OwnerCharacter.IsValid()) return;

    SetActorLocation(FMath::VInterpTo(GetActorLocation(), FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, GetActorLocation().Z) , DeltaTime, FollowSpeed));
}

void AArcaneField::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter.IsValid()) return;

    CheckBuff(true);
}

void AArcaneField::OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter.IsValid()) return;

    CheckBuff(false);
}

void AArcaneField::BindOverlap()
{
    FieldTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneField::OnBeginOverlap);
    FieldTrigger->OnComponentEndOverlap.AddDynamic(this, &AArcaneField::OnEndOverlap);
}

void AArcaneField::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_11);

    Super::DestroySKill();
}

void AArcaneField::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    FieldDamage = OwnerCharacter->GetCurrentATK() * DamageCoefficient;
    for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        CheckSilverEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
        CheckGoldEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
        CheckPlatinumEnhance(It.Key, It.Value);
    }

    HitPointComp->SetSlowPercent(SlowPercent);
    SetFieldDebuff(); SetFieldDamage();
    GetWorld()->GetTimerManager().SetTimer(FieldTimerHandle, this, &AArcaneField::SetFieldDebuff, DebuffRateTime, true);
    GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AArcaneField::SetFieldDamage, DamageRateTime, true);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneField::DestroySKill, DestroyTime, false);
    if(BuffType != EBuffType::None) BindOverlap();
    SetActorTickEnabled(true);
    // FieldTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneField::SetFieldDebuff);
}

void AArcaneField::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    float Max;
    switch (AbilityNum)
    {
        case 1: // 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DestroyTime, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 필드 크기 증가
        Max = SkillAbilityComponent->SizeUp_Multiple(AbilityData->Coefficient_X, NestingNum);
        SetActorScale3D(GetActorScale3D()* Max);
        break;

        case 3: // 도트뎀 증가
        Max = FieldDamage * 1.9f;
        SkillAbilityComponent->Coefficient_AddMultiple(FieldDamage, AbilityData->Coefficient_X, NestingNum);
        FieldDamage = FMath::Min(Max, FieldDamage);
        break;  
    }
}

void AArcaneField::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    DamageBuff = 1.0f;
    switch (AbilityNum)
    {
        case 1: // 스킬 데미지 증가
        SkillAbilityComponent->Coefficient_Add(DamageBuff, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::Damage;
        break;

        case 2: // 이동속도 감소
        bSlow = true;
        SkillAbilityComponent->Coefficient_Add(SlowPercent, AbilityData->Coefficient_X, NestingNum);
        SlowPercent = FMath::Min(90, SlowPercent);
        break;
        
        case 3: // 공격력 감소
        bWeak = true;
        SkillAbilityComponent->Coefficient_Add(WeakCoefficient, AbilityData->Coefficient_X, NestingNum);
        WeakCoefficient = FMath::Min(0.9, WeakCoefficient);
        break;
    }
}

void AArcaneField::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 아군 몬스터 공격
        break;

        case 2: // 이속 증가
        SkillAbilityComponent->Coefficient_Add(FastCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::Speed;
        break;

        case 3: // 공속 증가
        SkillAbilityComponent->Coefficient_Add(ATKSpeedCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATKSpeed;
        break;
        
        case 4: // 공격력 증가
        SkillAbilityComponent->Coefficient_Add(ATKCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATK;
        break;
    }
}