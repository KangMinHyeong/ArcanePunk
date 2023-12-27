// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber2.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Skill/SwordThrowBase.h"
#include "Components/Character/APSkillHubComponent.h"

void USkillNumber2::PlaySkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	if(bMark)
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(MarkedActor);
		if(!Enemy) return;
		OwnerCharacter->SetActorLocation(MarkingLocation);
		OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
		Enemy->TeleportMarkDeactivate();
		MarkErase();
	}
	else
	{
		auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(!OwnerAnim) return;
			
		OwnerAnim->PlaySkill_2_Montage();
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		
	}
}

void USkillNumber2::MarkingOn(FVector Location, AActor* OtherActor, float Time)
{
	bMark = true;
	MarkingLocation = Location;
	MarkedActor = OtherActor;
	GetWorld()->GetTimerManager().SetTimer(MarkTimerHandle, this, &USkillNumber2::MarkErase, Time, false);
}

void USkillNumber2::Activate_Skill2()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordThrow = GetWorld()->SpawnActor<ASwordThrowBase>(OwnerCharacter->GetSwordThrowClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*35.0f, OwnerCharacter->GetActorRotation() + FRotator(0,90.0f,0), SpawnParams);
	if(!SwordThrow) return;
	SwordThrow->SetOwner(OwnerCharacter);
}

void USkillNumber2::MarkErase()
{
	bMark = false;
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	GetWorld()->GetTimerManager().ClearTimer(MarkTimerHandle);
}
