
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"
#include "Character/SkillRange/APSkillRange.h"

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

void UArcanePunkCharacterAnimInstance::PlaySkill_1_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_1_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_2_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_2_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_3_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_3_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_4_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_4_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_5_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_5_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_6_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_6_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_7_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_7_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_1_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_1_Montage);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AttackTrigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;
    
    Character->GetAttackComponent()->NormalAttack(Character->GetActorLocation(), true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Active_Q()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber1()->Activate_Skill1();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_E_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber2()->Activate_Skill2();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_4_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->Activate_Skill4();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_Trigger()
{
    GetWorld()->GetTimerManager().SetTimer(Skill5_FireTimerHandle, this, &UArcanePunkCharacterAnimInstance::FireCheck, 0.05f, true);

}

void UArcanePunkCharacterAnimInstance::FireCheck()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    bool Check = false;

    switch (Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->SkillKey)
    {
        case ESkillKey::Q:
        if(!Character->GetOnQSkill()) Check = true;
        break;

        case ESkillKey::E:
        if(!Character->GetOnESkill()) Check = true;
        break;

        case ESkillKey::R:
        if(!Character->GetOnRSkill()) Check = true;
        break;
    }

    
    if(Check)
    {
        Montage_JumpToSection(TEXT("Fire"), Skill_5_Montage);
        GetWorld()->GetTimerManager().ClearTimer(Skill5_FireTimerHandle);
    } 
    // 후에 CC 맞으면 취소 되는것도 추가
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_FireStart()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    if(Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->SkillRange_Target) Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->SkillRange_Target->SetActorHiddenInGame(true);
    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->CharacterRotation();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_Fire()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->Activate_Skill5();
}
void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_FireEnd()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->Skill5_End();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_6_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber6()->Activate_Skill6();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_7_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->Activate_Skill7();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_UltSkill_1_Trigger()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetUltSkillNumber1()->Activate_UltSkill1();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_NextCombo()
{
    OnComboCheck.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimMove()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPMoveComponent()->AnimMovement();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimStop()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;

    Character->GetAPMoveComponent()->AnimMoveStop();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootRight()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;
    
    Character->GetSpawnFootPrintComponent()->SpawnFootPrint(true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootLeft()
{
    auto Character = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!Character || IsDead) return;
    
    Character->GetSpawnFootPrintComponent()->SpawnFootPrint(false);
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
