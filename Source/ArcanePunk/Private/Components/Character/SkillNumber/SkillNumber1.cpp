// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Skill/SwordImpact.h"

void USkillNumber1::PlaySkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_1_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber1::Activate_Skill1()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto SwordSkill = GetWorld()->SpawnActor<ASwordImpact>(OwnerCharacter->GetSwordImpactClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*150.0f, OwnerCharacter->GetActorRotation());
	if(!SwordSkill) return;
	SwordSkill->SetOwner(OwnerCharacter);
}