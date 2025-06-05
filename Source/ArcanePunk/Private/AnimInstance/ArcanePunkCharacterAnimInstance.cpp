
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"
#include "Character/SkillRange/APSkillRange.h"
#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// ys
#include "Logging/StructuredLog.h"
#include "MotionWarpingComponent.h"


DEFINE_LOG_CATEGORY(LogAnimInstance)

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

    OwnerCharacter->SetAttackRotation(200.0f);
    Montage_Play(Parrying_Montage);
}

void UArcanePunkCharacterAnimInstance::StopComboAttack()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    
    Montage_Stop(0.1f, Combo_Montage);
}

void UArcanePunkCharacterAnimInstance::PlayParryingSuccess_Montage()
{
    if(IsDead) return;
    if(Montage_IsPlaying(ParryingSuccess_Montage)) return;

    // OwnerCharacter->SetAttackRotation(200.0f);
    Montage_Play(ParryingSuccess_Montage);
}

//void UArcanePunkCharacterAnimInstance::PlayDash_Montage()
//{
//    //@캐릭터 사망 여부 확인
//    if (IsDead)
//    {
//        UE_LOGFMT(LogAnimInstance, Log, "캐릭터가 사망 상태이므로 대시 몽타주를 재생하지 않습니다.");
//        return;
//    }
//
//    //@소유 캐릭터 유효성 검사
//    if (!OwnerCharacter.IsValid())
//    {
//        UE_LOGFMT(LogAnimInstance, Error, "유효한 소유 캐릭터가 없어 대시 몽타주를 재생할 수 없습니다.");
//        return;
//    }
//
//    //@대시 몽타주 재생
//    Montage_Play(Dash_Montage);
//
//    UE_LOGFMT(LogAnimInstance, Log, "대시 몽타주 재생을 시작합니다.");
//}

void UArcanePunkCharacterAnimInstance::PlayDash_Montage()
{
    //@캐릭터 사망 여부 확인
    if (IsDead)
    {
        UE_LOGFMT(LogAnimInstance, Log, "캐릭터가 사망 상태이므로 대시 몽타주를 재생하지 않습니다.");
        return;
    }

    //@OwnerCharacter
    if (!OwnerCharacter.IsValid())
    {
        UE_LOGFMT(LogAnimInstance, Error, "유효한 소유 캐릭터가 없어 대시 몽타주를 재생할 수 없습니다.");
        return;
    }

    //@MovementComponent
    UAPMovementComponent* MovementComp = OwnerCharacter->FindComponentByClass<UAPMovementComponent>();
    if (!MovementComp)
    {
        UE_LOGFMT(LogAnimInstance, Error, "APMovementComponent를 찾을 수 없어 워프 타겟을 설정할 수 없습니다.");
        return;
    }

    //@MotionWarpingComponent
    UMotionWarpingComponent* MotionWarpComp = OwnerCharacter->FindComponentByClass<UMotionWarpingComponent>();
    if (!MotionWarpComp)
    {
        UE_LOGFMT(LogAnimInstance, Error, "MotionWarpingComponent를 찾을 수 없어 워프 타겟을 설정할 수 없습니다.");
        return;
    }

    //@대시 목적지 계산 - MovementComponent의 로직과 동일하게 구현
    FRotator PlayerRot = OwnerCharacter->GetActorRotation();
    FVector DashLocation = OwnerCharacter->GetActorLocation() + PlayerRot.Vector() * MovementComp->GetDashLength();

    UE_LOGFMT(LogAnimInstance, Log, "대시 워프 타겟 위치 계산: {0}, {1}, {2}",
        DashLocation.X, DashLocation.Y, DashLocation.Z);

    //@워프 타겟 설정 - "DashTarget"이라는 이름으로 위치만 지정
    FMotionWarpingTarget WarpTarget;
    WarpTarget.Name = FName("DashTarget");
    WarpTarget.Location = DashLocation;
    MotionWarpComp->AddOrUpdateWarpTarget(WarpTarget);

    //@몽타주 종료 델리게이트 바인딩
    OnMontageEnded.AddDynamic(this, &UArcanePunkCharacterAnimInstance::OnDashMontageEnded);

    //@몽타주 재생
    Montage_Play(Dash_Montage);

    UE_LOGFMT(LogAnimInstance, Log, "워프 타겟이 설정된 대시 몽타주 재생을 시작합니다.");

}

void UArcanePunkCharacterAnimInstance::StopDash_Montage()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;
    
    Montage_Stop(0.05f, Dash_Montage);
}

<<<<<<< HEAD
void UArcanePunkCharacterAnimInstance::OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    //@Dash Montage
    if (Montage != Dash_Montage)
    {
        return;
    }

    //@Owner Character
    if (!OwnerCharacter.IsValid())
    {
        UE_LOGFMT(LogAnimInstance, Error, "대시 몽타주 종료 시 유효한 소유 캐릭터가 없습니다.");
        OnMontageEnded.RemoveDynamic(this, &UArcanePunkCharacterAnimInstance::OnDashMontageEnded);
        return;
    }

    //@MovementComponent
    UAPMovementComponent* MovementComp = OwnerCharacter->FindComponentByClass<UAPMovementComponent>();
    if (!MovementComp)
    {
        UE_LOGFMT(LogAnimInstance, Error, "대시 몽타주 종료 시 APMovementComponent를 찾을 수 없습니다.");
        OnMontageEnded.RemoveDynamic(this, &UArcanePunkCharacterAnimInstance::OnDashMontageEnded);
        return;
    }

    // 대시 종료 처리 호출
    UE_LOGFMT(LogAnimInstance, Log, "대시 몽타주 종료: MovementComponent의 EndDash 호출");
    MovementComp->EndDash();

    // 델리게이트 바인딩 해제
    OnMontageEnded.RemoveDynamic(this, &UArcanePunkCharacterAnimInstance::OnDashMontageEnded);
}

void UArcanePunkCharacterAnimInstance::PlaySwapSkill_Retreat()
=======
void UArcanePunkCharacterAnimInstance::PlaySwapSkill_Exit()
>>>>>>> origin
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;

    Montage_Play(SwapSkill_Retreat);
}

void UArcanePunkCharacterAnimInstance::PlaySwapSkill_Entry()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;

    Montage_Play(SwapSkill_Sally);
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

void UArcanePunkCharacterAnimInstance::AnimNotify_SpawnSwordSound_1()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;

    OwnerCharacter->SpawnAttackSound(1);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_SpawnSwordSound_2()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;

    OwnerCharacter->SpawnAttackSound(2);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_SpawnSwordSound_3()
{
    if(IsDead) return;
    if(!OwnerCharacter.IsValid()) return;

    OwnerCharacter->SpawnAttackSound(3);
}

void UArcanePunkCharacterAnimInstance::AnimNotify_AttackVoiceSound()
{
    if(!OwnerCharacter.IsValid() || IsDead) return;

    OwnerCharacter->SpawnAttackVoiceSound();
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

void UArcanePunkCharacterAnimInstance::AnimNotify_Skill_ChargingEnd()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    if(OwnerCharacter->OnSkillChargingEndTrigger.IsBound()) OwnerCharacter->OnSkillChargingEndTrigger.Broadcast();
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

    // OwnerPlayer->GetRSkillNumber()->Activate_Skill();
}

void UArcanePunkCharacterAnimInstance::AnimNotify_UltSkill_Charge_Trigger()
{
    if(!OwnerPlayer.IsValid() || IsDead) return;

    // OwnerPlayer->GetRSkillNumber()->Enhance();
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

    OwnerCharacter->GetAPMoveComponent()->StopTickMove();
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

}
