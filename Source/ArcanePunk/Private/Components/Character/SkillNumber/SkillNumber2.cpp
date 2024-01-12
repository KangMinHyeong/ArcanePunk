// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber2.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Skill/SwordThrowBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/CapsuleComponent.h"

void USkillNumber2::PlaySkill(uint8 SkillType)
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
		OwnerCharacter->SetDoing(true);
		if(SkillType == 2)
		{
			if(CheckSmartKey(SkillType, OwnerCharacter))
			{
				OnSkill(SkillType);
			}
			else
			{
				auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
				SetMouseCursor(PC, ECursorType::Crosshairs);
				PC->DisplayHomingUI(2, SkillType);
			}
		}
		else
		{
			OnSkill(SkillType);
		}
	}
}

void USkillNumber2::OnSkill(uint8 SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	CurrentSkillType = SkillType;
			
	OwnerAnim->PlaySkill_2_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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

	auto SwordThrow = GetWorld()->SpawnActor<ASwordThrowBase>(OwnerCharacter->GetSwordThrowClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*35.0f, OwnerCharacter->GetActorRotation() + FRotator(0,90.0f,0), SpawnParams);
	if(SwordThrow) SwordThrow->SetSkillType(CurrentSkillType);
}

void USkillNumber2::MarkErase()
{
	bMark = false;
	GetWorld()->GetTimerManager().ClearTimer(MarkTimerHandle);
}

