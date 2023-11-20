// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Character/ArcanePunkCharacter.h"

UArcanePunkCharacterAnimInstance::UArcanePunkCharacterAnimInstance()
{
    CurrentPawnSpeed = 0.0f;
    IsDead = false;
}

void UArcanePunkCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	auto Character = TryGetPawnOwner();
	if (!Character) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Character->GetVelocity().Size();
		// auto Character = Cast<ACharacter>(Pawn);
		// if (Character)
		// {
		// 	IsInAir = Character->GetMovementComponent()->IsFalling();
		// }
	}
}

void UArcanePunkCharacterAnimInstance::PlayAttack_A_Montage()
{
    if(IsDead) return;
    Montage_Play(Attack_A_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayAttack_B_Montage()
{
    if(IsDead) return;
    Montage_Play(Attack_B_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_Q_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_Q_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_E_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_E_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_R_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_R_Montage);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AttackTrigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->NormalAttack();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Active_Q()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->Activate_Q();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_E_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->Activate_E();
}
