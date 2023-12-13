// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber3.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void USkillNumber3::PlaySkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!MyController) return;

	if(!bSkill3)
	{
		bSkill3 = true;
		
		MyController->SetActivate_Skill3(bSkill3);
	}
	else
	{
		bSkill3 = false;
		MyController->SetActivate_Skill3(bSkill3);
	}
}

void USkillNumber3::Cast_Skill3(FVector HitLocation)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnerCharacter->Skill3_Effect, OwnerCharacter->Skill3_Location, FRotator::ZeroRotator, OwnerCharacter->Skill3_Size);
	if(OwnerCharacter->GetAttackComponent()) OwnerCharacter->GetAttackComponent()->NormalAttack(HitLocation, false, 0.4f);
	OwnerAnim->PlaySkill_3_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber3::SetSkill3(bool NewBool)
{
	bSkill3 = NewBool;
}
