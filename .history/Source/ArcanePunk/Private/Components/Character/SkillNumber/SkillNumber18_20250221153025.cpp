
#include "Components/Character/SkillNumber/SkillNumber18.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Skill/SpinSlash.h"

USkillNumber18::USkillNumber18()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_18");
}

void USkillNumber18::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber18::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(bActivate) return;

	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;
        Spawn_SkillRange();
		OnSkill();
	}
}

void USkillNumber18::Spawn_SkillRange()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();
    
    Skill18_LimitDistance =( OwnerCharacter->GetAttackComponent()->GetBaseAttackDist() + OwnerCharacter->GetAttackComponent()->GetBaseAttackRadius()) + 20.0f;
	ActivateSkillRange_Target(Skill18_LimitDistance, Skill18_LimitDistance, ESkillRangeType::Square);
	if(SkillRange_Target.IsValid()) 
    {
        SkillRange_Target->SetMaxDist(0.0f); // 최대 사거리 0.5배 해줘야지 같은 거리로 뒤로 밀림
	    SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	
        SkillRange_Target->SetMouseControll(false);
    }
}

void USkillNumber18::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_18_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    Activate_Skill();
}

void USkillNumber18::Activate_Skill()
{
	Super::Activate_Skill();
    if(!OwnerCharacter.IsValid()) return;

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TWeakObjectPtr<ASpinSlash> SpinSlash = GetWorld()->SpawnActor<ASpinSlash>(OwnerCharacter->GetAPSkillHubComponent()->GetSpinSlash(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!SpinSlash.IsValid()) return; 
	SpinSlash->SetOwner(OwnerCharacter.Get());
    SpinSlash->SetSlashWidth(Skill18_LimitDistance);
    SpinSlash->SetSkill(SkillAbilityNestingData, this);	

    SpinSlash->OnCharging();

	OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &USkillNumberBase::Activate_Skill);
	
}

void USkillNumber18::SkillEnd()
{
	Super::SkillEnd();
    if(!OwnerCharacter.IsValid()) return;
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->StopSkill_18_Montage();

    Remove_Skill();
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void USkillNumber18::UpdateSkillData()
{
}
