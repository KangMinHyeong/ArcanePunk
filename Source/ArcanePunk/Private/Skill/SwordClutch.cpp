
#include "Skill/SwordClutch.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/Sub/ClutchTrigger.h"
#include "Components/SphereComponent.h"

ASwordClutch::ASwordClutch()
{
    ClutchRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ClutchRoot"));

    SetRootComponent(ClutchRoot);

    SkillCategory = ESkillCategory::Spawn_Dynamic;
}

void ASwordClutch::BeginPlay()
{
    Super::BeginPlay();  
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
}

void ASwordClutch::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(OwnerCharacter.IsValid())
    {
        SetActorLocation(OwnerCharacter->GetActorLocation());
        SetActorRotation(FMath::RInterpTo(GetActorRotation(),FRotator(0, GetActorRotation().Yaw + 90.0f, 0), DeltaTime, ClutchSpeed));
    }
}

void ASwordClutch::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_14);
    Super::DestroySKill();

}

void ASwordClutch::SpawnClutchTrigger()
{
    if(!OwnerCharacter.IsValid()) return;
    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TArray<float> AddLocation_X; TArray<float> AddLocation_Y; AddLocation_X.SetNum(SwordNumber); AddLocation_Y.SetNum(SwordNumber);
    float Degree = 360.0f / SwordNumber; Degree = FMath::DegreesToRadians(Degree);

    for(int32 i = 0; i<SwordNumber; i++)
    {
        AddLocation_X[i] = FMath::Cos(Degree * i); AddLocation_Y[i] =  FMath::Sin(Degree * i);
        FVector AddLocation = FVector(AddLocation_X[i], AddLocation_Y[i], 0.0f) * Distance;

        auto ClutchTrigger = GetWorld()->SpawnActor<AClutchTrigger>(ClutchTriggerClass, GetActorLocation() + AddLocation, GetActorRotation(), SpawnParams);
        if(ClutchTrigger) 
        {
            ClutchTrigger->SetOwner(this); 
            ClutchTrigger->AttachToComponent( ClutchRoot, FAttachmentTransformRules ::KeepWorldTransform);
            ClutchTrigger->SetClutchSphere(Wide/2.0f);
            ClutchTrigger->SetDestroy(DestroyTime);
            ClutchTrigger->SetSlowPercent(SlowPercent);
            ClutchTrigger->SetSkill(SkillAbilityData, SkillComp.Get());
        }
    }
    
}

void ASwordClutch::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent); // Wide 곱해주기 추가
    if(!OwnerCharacter.IsValid()) return;
    
    CurrentDamage = OwnerCharacter->GetCurrentATK() * DamageCoefficient;
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

    SpawnClutchTrigger();
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASwordClutch::DestroySKill, DestroyTime, false);
    SetActorTickEnabled(true);
}

void ASwordClutch::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DestroyTime, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 공격력 감소, 슬로우 증가
        bSlow = true;
        SkillAbilityComponent->Coefficient_SubtractMultiple(CurrentDamage, AbilityData->Coefficient_X, NestingNum);
        SkillAbilityComponent->Coefficient_Add(SlowPercent, AbilityData->Coefficient_Y, NestingNum);
        break;

    }
}

void ASwordClutch::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 추가 단검
        SkillAbilityComponent->Coefficient_Add(SwordNumber, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 단검 속도 증가
        SkillAbilityComponent->Coefficient_Add(ClutchSpeed, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void ASwordClutch::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 생명력 흡수
        bDrain = true;
        DrainCoefficient = AbilityData->Coefficient_X;
        break;

        case 2: // 지속시간 증가
        bDurationUpgrade = true;
        SkillAbilityComponent->Coefficient_Add(DurationUpgradeCoefficient, AbilityData->Coefficient_X, NestingNum);
        break;
        
    }
}
