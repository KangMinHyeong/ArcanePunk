// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"

UArcanePunkCharacterAnimInstance::UArcanePunkCharacterAnimInstance()
{
    CurrentPawnSpeed = 0.0f;
    IsDead = false;
}

void UArcanePunkCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
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
    Montage_Play(Attack_A_Montage, 1.0f);
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
    
    Character->GetAttackComponent()->NormalAttack(Character->GetActorLocation(), true);
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

void UArcanePunkCharacterAnimInstance::AnimNotify_NextCombo()
{
    OnComboCheck.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimMove()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->GetAPMoveComponent()->AnimMovement();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimStop()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->GetAPMoveComponent()->AnimMoveStop();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootRight()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;
    
    Character->SpawnFootPrint(true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootLeft()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;
    
    Character->SpawnFootPrint(false);
}

void UArcanePunkCharacterAnimInstance::JumpToComboSection(int32 NewSection)
{
    if(IsDead) return;
    AttackSection = NewSection;
	if(!Montage_IsPlaying(Attack_A_Montage)) return;
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), Attack_A_Montage);
}

FName UArcanePunkCharacterAnimInstance::GetAttackMontageSectionName(int32 Section)
{
    if(Section >= 1 && Section <= 3)
    {
        return FName(*FString::Printf(TEXT("Attack%d"), Section));
    }
	return FName(TEXT(""));
}

int32 UArcanePunkCharacterAnimInstance::GetAttackSection()
{
    return AttackSection;
}
