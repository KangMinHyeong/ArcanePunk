
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"
#include "Character/SkillRange/APSkillRange.h"
#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UArcanePunkCharacterAnimInstance::UArcanePunkCharacterAnimInstance()
{
    CurrentPawnSpeed = 0.0f;
    IsDead = false;
}

void UArcanePunkCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if(!OwnerCharacter.IsValid() || IsDead) return;

	CurrentPawnSpeed = OwnerCharacter->GetVelocity().Size();	
}

void UArcanePunkCharacterAnimInstance::NativeBeginPlay()
{
    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;

    if(GM->IsBattleStage())
    {
        bBattleMode = 1.0f;
    }
    else
    {
        bBattleMode = 0.0f;
    }

    OwnerCharacter = Cast<AAPCharacterBase>(TryGetPawnOwner()); 
    OwnerPlayer = Cast<AArcanePunkCharacter>(TryGetPawnOwner()); 

    OnUltChargeEnd.AddUObject(this, &UArcanePunkCharacterAnimInstance::JumpToFireMontage_Ult);
}

void UArcanePunkCharacterAnimInstance::PlayCombo_Montage(float AttackCancelTime)
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    if(Montage_IsPlaying(Combo_Montage)) return;
    OwnerCharacter->SetAttackRotation();
    Montage_Play(Combo_Montage, OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed, EMontagePlayReturnType::MontageLength, AttackCancelTime);
}

void UArcanePunkCharacterAnimInstance::PlayParrying_Montage()
{
    if(IsDead) return;
    if(Montage_IsPlaying(Parrying_Montage)) return;

    OwnerCharacter->SetAttackRotation();
    Montage_Play(Parrying_Montage);
}

void UArcanePunkCharacterAnimInstance::StopComboAttack()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    
    Montage_Stop(0.1f, Combo_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayDash_Montage()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    
    Montage_Play(Dash_Montage);
}

void UArcanePunkCharacterAnimInstance::StopDash_Montage()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    
    Montage_Stop(0.05f, Dash_Montage);
}

void UArcanePunkCharacterAnimInstance::JumpToComboSection(int32 NewSection)
{
    if(IsDead) return;
    AttackSection = NewSection;
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetAttackRotation();

    // switch (NewSection)
    // {
    //     case 2:
    //     Montage_Play(Combo_2_Montage, OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed);
    //     break;

    //     case 3:
    //     Montage_Play(Combo_3_Montage, OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed);
    //     break;
    // }
	// Montage_JumpToSection(GetAttackMontageSectionName(NewSection), Attack_A_Montage);
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

void UArcanePunkCharacterAnimInstance::PlaySkill_15_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_15_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_18_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_18_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_19_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_19_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_20_Montage()
{
    if(IsDead) return;
    Montage_Play(Skill_20_Montage);
}

void UArcanePunkCharacterAnimInstance::StopSkill_5_Montage()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;
    Montage_Stop(0.0f, Skill_5_Montage);
}

void UArcanePunkCharacterAnimInstance::StopSkill_5_Fire_Montage()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;
    Montage_JumpToSection(TEXT("End"), Skill_5_Fire_Montage);
}

void UArcanePunkCharacterAnimInstance::StopSkill_18_Montage()
{
    if(IsDead) return;
    Montage_Stop(0.5f, Skill_18_Montage);
}

void UArcanePunkCharacterAnimInstance::PlaySkill_5_Fire_Montage()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;
    Montage_Play(Skill_5_Fire_Montage);

    // TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerPlayer->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);
	// if(SkillNum.IsValid())
	// {
    //     if(SkillNum->SkillRange_Target.IsValid()) 
    //     {
    //         SkillNum->SkillRange_Target->SetActorHiddenInGame(true);
    //         SkillNum->SetComponentTickEnabled(false);
    //         SkillNum->CharacterRotation();
    //     }
	// } 
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_1_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_1_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_2_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_2_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_3_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_3_Montage);
}

void UArcanePunkCharacterAnimInstance::StopUltSkill_3_Montage()
{
    if(IsDead) return;
    Montage_Stop(0.0f, UltSkill_3_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_4_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_4_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_5_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_5_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_6_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_6_Montage);
}

void UArcanePunkCharacterAnimInstance::StopUltSkill_6_Montage()
{
    if(IsDead) return;
    Montage_Stop(0.0f, UltSkill_6_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_8_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_8_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_9_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_9_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_10_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_10_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_11_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_11_Montage);
}

void UArcanePunkCharacterAnimInstance::StopUltSkill_11_Montage()
{
    if(IsDead) return;
    Montage_Stop(0.0f, UltSkill_11_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_12_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_12_Montage);
}

void UArcanePunkCharacterAnimInstance::StopUltSkill_12_Montage()
{
    if(IsDead) return;
    Montage_Stop(0.0f, UltSkill_12_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_13_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_13_Montage, 0.7f);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_14_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_14_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_15_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_15_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayUltSkill_17_Montage()
{
    if(IsDead) return;
    Montage_Play(UltSkill_17_Montage);
}

void UArcanePunkCharacterAnimInstance::JumpToFireMontage_Ult()
{
    if(IsDead) return;
    Montage_Play(UltSkill_Fire_Montage);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AttackTrigger()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAttackComponent()->NormalAttack(OwnerCharacter->GetActorLocation(), true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_MultiAttackTrigger()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAttackComponent()->MultiAttack();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_SwordTrail_1()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    // OwnerCharacter->GetAttackComponent()->SpawnSwordTrail(1);
}
void UArcanePunkCharacterAnimInstance::AnimNotify_SwordTrail_2()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    // OwnerCharacter->GetAttackComponent()->SpawnSwordTrail(2);
}
void UArcanePunkCharacterAnimInstance::AnimNotify_SwordTrail_3()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAttackComponent()->SpawnSwordTrail(3);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_ParryingEnd()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAttackComponent()->SetParrying(false);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_Trigger()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    if(OwnerCharacter->OnSkillTrigger.IsBound()) OwnerCharacter->OnSkillTrigger.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_Charging()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    if(OwnerCharacter->OnSkillChargingTrigger.IsBound()) OwnerCharacter->OnSkillChargingTrigger.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_Enhance()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    if(OwnerCharacter->OnSkillEnhanceTrigger.IsBound()) OwnerCharacter->OnSkillEnhanceTrigger.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_End()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    if(OwnerCharacter->OnSkillEndTrigger.IsBound()) OwnerCharacter->OnSkillEndTrigger.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_UltSkill_Trigger()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetRSkillNumber()->Activate_Skill();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_UltSkill_Charge_Trigger()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetRSkillNumber()->Enhance();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_NextCombo()
{
    OnComboCheck.Broadcast();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimMove()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAPMoveComponent()->ComboMovement();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AnimStop()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->GetAPMoveComponent()->ComboMoveStop();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootRight()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetSpawnFootPrintComponent()->SpawnFootPrint(true);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootRightOnlySound()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetSpawnFootPrintComponent()->SpawnFootPrint(true, false);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootLeft()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetSpawnFootPrintComponent()->SpawnFootPrint(false);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_FootLeftOnlySound()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    OwnerPlayer->GetSpawnFootPrintComponent()->SpawnFootPrint(false, false);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_HideClear()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    if(!OwnerPlayer->IsEnhanceTent()) OwnerPlayer->HideClear();
}
