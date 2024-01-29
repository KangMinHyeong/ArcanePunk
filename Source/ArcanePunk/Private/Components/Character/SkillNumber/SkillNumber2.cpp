// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber2.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Skill/SwordThrowBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/CapsuleComponent.h"

void USkillNumber2::BeginPlay()
{
	Super::BeginPlay();

}

void USkillNumber2::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	EnableSkillAbilityList.Add(ESkillAbility::Homing);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber2::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	if(bMark)
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(MarkedActor);
		if(!Enemy) return;
		OwnerCharacter->SetActorLocation(Enemy->GetActorLocation() - Enemy->GetActorForwardVector() * Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.5f);
		Enemy->TeleportMarkDeactivate();
		MarkErase();
	}
	else
	{
		CurrentSkillType = SkillType;
		SetAbility(WhichKey);
		OwnerCharacter->SetDoing(true);
		if(CurrentSkillAbility.Contains(ESkillAbility::Homing))
		{
			if(CheckSmartKey(WhichKey, OwnerCharacter))
			{
				OnSkill();
			}
			else
			{
				auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
				SetMouseCursor(PC, ESkillCursor::Crosshairs);
				PC->DisplayHomingUI(ESkillNumber::Skill_2);
			}
		}
		else
		{
			OnSkill();
		}
	}
}

void USkillNumber2::OnSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_2_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber2::Remove_Skill()
{
}

void USkillNumber2::MarkingOn(AActor* OtherActor, float Time)
{
	bMark = true;
	MarkedActor = OtherActor;
	GetWorld()->GetTimerManager().SetTimer(MarkTimerHandle, this, &USkillNumber2::MarkErase, Time, false);
}

void USkillNumber2::Activate_Skill2()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordThrow = GetWorld()->SpawnActor<ASwordThrowBase>(OwnerCharacter->GetSwordThrowClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	if(SwordThrow) SwordThrow->SetSkill(CurrentSkillType, CurrentSkillAbility);
}

void USkillNumber2::MarkErase()
{
	bMark = false;
	GetWorld()->GetTimerManager().ClearTimer(MarkTimerHandle);
}

