
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

    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    if (!IsDead)
	{
		CurrentPawnSpeed = OwnerCharacter->GetVelocity().Size();
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

void UArcanePunkCharacterAnimInstance::PlaySkill_8_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_8_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_9_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_9_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_10_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_10_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_11_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_11_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_12_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_12_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_13_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_13_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_14_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_14_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_1_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_1_Montage);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AttackTrigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAttackComponent()->NormalAttack(OwnerCharacter->GetActorLocation(), true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_1_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_1);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_2_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_2);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_4_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_4);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_Trigger()
{
    GetWorld()->GetTimerManager().SetTimer(Skill5_FireTimerHandle, this, &UArcanePunkCharacterAnimInstance::FireCheck, 0.05f, true);

}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_Enhance()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;
    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	if(SkillNum.IsValid())
	{
		SkillNum->Enhance();
	} 
}

void UArcanePunkCharacterAnimInstance::FireCheck()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    bool Check = false;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	if(SkillNum.IsValid())
	{
        switch (SkillNum->SkillKey)
        {
            case ESkillKey::Q:
            if(!OwnerCharacter->GetOnQSkill()) Check = true;
            break;

            case ESkillKey::E:
            if(!OwnerCharacter->GetOnESkill()) Check = true;
            break;

            case ESkillKey::R:
            if(!OwnerCharacter->GetOnRSkill()) Check = true;
            break;
        }
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
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	if(SkillNum.IsValid())
	{
        if(SkillNum->SkillRange_Target.IsValid()) SkillNum->SkillRange_Target->SetActorHiddenInGame(true);
        SkillNum->CharacterRotation();
	} 

}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_Fire()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}
void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_5_FireEnd()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	if(SkillNum.IsValid())
	{
		SkillNum->SkillEnd();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_6_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_6);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_7_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_7);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_8_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_8);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_9_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_9);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_10_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_10);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_11_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_11);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_12_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_12);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_13_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_13);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_14_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_14);
	if(SkillNum.IsValid())
	{
		SkillNum->Activate_Skill();
	} 
}

void UArcanePunkCharacterAnimInstance::AnimNotify_UltSkill_Trigger()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetRSkillNumber()->Activate_Skill();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_NextCombo()
{
    OnComboCheck.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimMove()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAPMoveComponent()->AnimMovement();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimStop()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAPMoveComponent()->AnimMoveStop();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootRight()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetSpawnFootPrintComponent()->SpawnFootPrint(true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootLeft()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetSpawnFootPrintComponent()->SpawnFootPrint(false);
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

void UArcanePunkCharacterAnimInstance::AnimNotify_HideClear()
{
    TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(TryGetPawnOwner());
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->HideClear();
}
