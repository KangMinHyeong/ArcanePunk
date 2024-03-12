// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber14.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/SwordClutch.h"

void USkillNumber14::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
}

void USkillNumber14::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
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
            // if(ArcaneBall) ArcaneBall->Explosion();
			// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
        }
        else
        {
            OwnerCharacter->SetDoing(true);
            SetAbility(WhichKey);
            SkillKey = WhichKey;
            Skilling = true;
            CurrentSkillType = SkillType;
            Spawn_Skill14();
        }
	}
}

void USkillNumber14::Spawn_Skill14()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	OwnerCharacterPC->bShowMouseCursor = true;
	CursorImmediately();

	if(!CheckSmartKey(SkillKey, OwnerCharacter)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_14);}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ActivateSkillRange_Target(Skill14_LimitDistance, Skill14_LimitDistance, ESkillRangeType::Around_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill14_LimitDistance+Skill14_Wide/2);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetWide(Skill14_Wide);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(CurrentSkillType, CurrentSkillAbility);

	ActivateSkillRange_Round(Skill14_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(CurrentSkillType, CurrentSkillAbility);

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber14::OnSkill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlaySkill_14_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
}

void USkillNumber14::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber14::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

    OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto SwordClutch = GetWorld()->SpawnActor<ASwordClutch>(OwnerCharacter->GetSwordClutchClass(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!SwordClutch) return;
	SwordClutch->SetOwner(OwnerCharacter);
	if(SkillRange_Target.IsValid()) SwordClutch->SetDistance(SkillRange_Target->GetTargetDistance() - Skill14_Wide/2);
	if(SkillRange_Target.IsValid()) SwordClutch->SetClutchSpeed(SkillRange_Target->GetMaxDist() / SkillRange_Target->GetTargetDistance());
	SwordClutch->SetWide(Skill14_Wide);
	SwordClutch->SetSkill(CurrentSkillType, CurrentSkillAbility);
	Remove_Skill();
	bActivate = true;
}
