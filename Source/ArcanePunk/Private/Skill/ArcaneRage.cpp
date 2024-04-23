// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ArcaneRage.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "UserInterface/APHUD.h"

AArcaneRage::AArcaneRage()
{
    RageRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("RageRoot"));
    RageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RageEffect"));

    SetRootComponent(RageRoot);
    RageEffect->SetupAttachment(RageRoot);

    RageRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillCategory = ESkillCategory::Buff;
}

void AArcaneRage::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneRage::SetRageMode()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetRageMode(true);
    if(BuffType == EBuffType::Speed) {OwnerCharacter->GetCrowdControlComponent()->FastState(FastCoefficient, DestroyTime);}
    if(BuffType == EBuffType::ATKSpeed) {OwnerCharacter->GetBuffComp()->ATKSpeedUp(ATKSpeedCoefficient, DestroyTime); }
    if(BuffType == EBuffType::ATK) {OwnerCharacter->GetBuffComp()->ATKUp(ATKCoefficient, DestroyTime);}
}

void AArcaneRage::DestroySKill()
{
    if(OwnerCharacter.IsValid()) OwnerCharacter->SetRageMode(false);

    DeActivate(ESkillNumber::Skill_12);
    if(bPluslusInstantDeath) OwnerCharacter->GetAttackComponent()->SetBaseInstantDeathPercent(OwnerCharacter->GetAttackComponent()->GetBaseInstantDeathPercent() - PlusInstantDeathPercent );
    if(ReflectingModeGauge > 0) OwnerCharacter->SetReflectMode(OwnerCharacter->GetReflectMode() -ReflectingModeGauge);
    if(bHPRecovery) ApplyRecovery();
    Super::DestroySKill();
}

void AArcaneRage::ApplyRecovery()
{
    if(!OwnerCharacter.IsValid()) return;

    auto PDD = OwnerCharacter->GetPlayerStatus(); float OriginHP = PDD.PlayerDynamicData.HP;
    float HP = PDD.PlayerDynamicData.HP + (PDD.PlayerDynamicData.MaxHP - PDD.PlayerDynamicData.HP)*HPRecoveryPercent; 
    PDD.PlayerDynamicData.HP = FMath::Min(PDD.PlayerDynamicData.MaxHP, HP);

    OwnerCharacter->SetDefaultHP(PDD.PlayerDynamicData.HP); 
    OwnerCharacter->GetAPHUD()->OnUpdateHPBar.Broadcast(OriginHP);
}

void AArcaneRage::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    if(!OwnerCharacter.IsValid()) return;

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
    
    SetRageMode();
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneRage::DestroySKill, DestroyTime, false);
}

void AArcaneRage::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DestroyTime, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 쿨타임 감소 , 이미 처리
        
        break;

    }
}

void AArcaneRage::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 이속 증가
        SkillAbilityComponent->Coefficient_Add(FastCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::Speed;
        break;

        case 2: // 공속 증가
        bSlow = true;
        SkillAbilityComponent->Coefficient_Add(ATKSpeedCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATKSpeed;
        break;
        
        case 3: // 공격력 증가
        bWeak = true;
        SkillAbilityComponent->Coefficient_Add(ATKCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATK;
        break;
    }
}

void AArcaneRage::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 즉사
        bPluslusInstantDeath = true;
        SkillAbilityComponent->Coefficient_Add(PlusInstantDeathPercent, AbilityData->Coefficient_X, NestingNum);
        PlusInstantDeathPercent = FMath::Min(50.0f, PlusInstantDeathPercent);
        OwnerCharacter->GetAttackComponent()->SetBaseInstantDeathPercent(OwnerCharacter->GetAttackComponent()->GetBaseInstantDeathPercent() + PlusInstantDeathPercent);
        break;

        case 2: // 리플렉팅
        ReflectingModeGauge = 1;
        OwnerCharacter->SetReflectMode(OwnerCharacter->GetReflectMode() + ReflectingModeGauge);
        break;

        case 3: // 체력회복
        bHPRecovery = true;
        SkillAbilityComponent->Coefficient_Add(HPRecoveryPercent, AbilityData->Coefficient_X, NestingNum);
        HPRecoveryPercent = FMath::Min(1.0f, HPRecoveryPercent);
        break;
    }
}