// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber13.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneTent.h"

void USkillNumber13::AddAbilityList()
{
    EnableSkillAbilityList.Add(ESkillAbility::Gigant);
}

void USkillNumber13::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(bActivate) return;

	bActivate = true;
    OwnerCharacter->SetDoing(true);
	SetAbility(WhichKey);
    SkillKey = WhichKey;
	Skilling = true;
	CurrentSkillType = SkillType;
	OnSkill();
}

void USkillNumber13::OnSkill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlaySkill_13_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber13::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

    OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneTent = GetWorld()->SpawnActor<AArcaneTent>(OwnerCharacter->GetArcaneTentClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneTent) return;
	ArcaneTent->SetOwner(OwnerCharacter);
	ArcaneTent->SetSkill(CurrentSkillType, CurrentSkillAbility);
    
}
