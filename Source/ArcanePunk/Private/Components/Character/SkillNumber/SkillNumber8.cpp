// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber8.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneBall.h"

void USkillNumber8::BeginPlay()
{
	Super::BeginPlay();
	SkillAbilityNestingData.SkillName = TEXT("Skill_8");
}

void USkillNumber8::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber8::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
        if(bActivate)
        {
            if(ArcaneBall.IsValid()) ArcaneBall->Explosion();
            bActivate = false;
        }
        else
        {
            OwnerCharacter->SetDoing(true);
            SetAbility(WhichKey);
            SkillKey = WhichKey;
            Skilling = true;
            CurrentSkillType = SkillType;
            Spawn_Skill8();
        }
	}
}

void USkillNumber8::Spawn_Skill8()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    if(!CheckSmartKey(SkillKey, OwnerCharacter)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_8);}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	ActivateSkillRange_Target(Skill8_Wide, Skill8_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill8_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey, OwnerCharacter))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber8::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_8_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
    CharacterRotation();
}

void USkillNumber8::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber8::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    SpawnAddLocation = Skill8_Wide;

	ArcaneBall = GetWorld()->SpawnActor<AArcaneBall>(OwnerCharacter->GetArcaneBallClass(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation());
	if(!ArcaneBall.IsValid()) return; 
    ArcaneBall->SetOwner(OwnerCharacter);
    ArcaneBall->SetBallRadius(Skill8_Wide);
	ArcaneBall->SetSkill(SkillAbilityNestingData);	
    float Size = ArcaneBall->GetActorScale3D().Y/ ArcaneBall->DefaultSize;
    // ArcaneBall->SetActorLocation( ArcaneBall->GetActorLocation() + (OwnerCharacter->GetActorUpVector()*150.0f*Size));
    ArcaneBall->SetDeadTime((SkillRange_Target->GetTargetDistance() * 2.0f - SpawnAddLocation) / ArcaneBall->GetBallSpeed()); // 거리 = 속력 * 시간 으로 DeadTime 수정
	// if(SkillRange_Target) ArcaneBeam->SetDistance(SkillRange_Target->GetTargetDistance() * 2.0f);
	// if(SkillRange_Target) ArcaneBeam->SetWide(SkillRange_Target->GetTargetWide());
	Remove_Skill();
    bActivate = true;

    OwnerCharacter->SetDoing(false);
}

void USkillNumber8::SkillEnd()
{
    bActivate = false;
}
