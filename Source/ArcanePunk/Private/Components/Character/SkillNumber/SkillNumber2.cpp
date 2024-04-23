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

USkillNumber2::USkillNumber2()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_2");
}

void USkillNumber2::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber2::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber2::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate)
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(MarkedActor);
		if(!Enemy) return;
		OwnerCharacter->SetActorLocation(Enemy->GetActorLocation() - Enemy->GetActorForwardVector() * Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.5f);
		Enemy->TeleportMarkDeactivate();
		MarkErase();
	}
	else
	{
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
		OwnerCharacter->SetDoing(true);

		OnSkill();
	}
}

void USkillNumber2::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_2_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber2::Remove_Skill()
{
}

void USkillNumber2::MarkingOn(AActor* OtherActor, float Time)
{
	bActivate = true;
	MarkedActor = OtherActor;
	GetWorld()->GetTimerManager().SetTimer(MarkTimerHandle, this, &USkillNumber2::MarkErase, Time, false);
}

void USkillNumber2::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordThrow = GetWorld()->SpawnActor<ASwordThrowBase>(OwnerCharacter->GetAPSkillHubComponent()->GetSwordThrowClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	// if(SwordThrow) SwordThrow->SetSkill(CurrentSkillType, CurrentSkillAbility);
}

void USkillNumber2::MarkErase()
{
	bActivate = false;
	GetWorld()->GetTimerManager().ClearTimer(MarkTimerHandle);
}

void USkillNumber2::UpdateSkillData()
{
}
