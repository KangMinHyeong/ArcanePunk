
#include "Components/Character/SkillNumber/SkillNumber15.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber15::USkillNumber15()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_15");
}

void USkillNumber15::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber15::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void USkillNumber15::OnSkill()
{
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_15_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber15::Activate_Skill()
{
	Super::Activate_Skill();
    if(!OwnerCharacter.IsValid()) return;

    auto PDD = OwnerCharacter->GetPlayerStatus(); float OriginHP = PDD.StatusData.HP;
    float HP = PDD.StatusData.HP + (PDD.StatusData.MaxHP - PDD.StatusData.HP)*HPRecoveryPercent; 
    PDD.StatusData.HP = FMath::Min(PDD.StatusData.MaxHP, HP);

    OwnerCharacter->SetDefaultHP(PDD.StatusData.HP); 
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->SetHPPercent(OriginHP);

    UNiagaraFunctionLibrary::SpawnSystemAttached(OwnerCharacter->GetAPSkillHubComponent()->GetHealingEffect(), OwnerCharacter->GetRootComponent(), TEXT("TerminatorEffect"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(1,1,1), EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);

    SkillEnd();
}

void USkillNumber15::SkillEnd()
{
	Super::SkillEnd();
    Remove_Skill();
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void USkillNumber15::UpdateSkillData()
{
}