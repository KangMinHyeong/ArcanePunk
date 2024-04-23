
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
		Remove_Skill();
	}
	else
	{
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
		OwnerCharacter->SetDoing(true);
		Skilling = true;
        Spawn_Skill18();
		OnSkill();
	}
}

void USkillNumber18::Spawn_Skill18()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();
    
    Skill18_LimitDistance =( OwnerCharacter->GetAttackComponent()->GetBaseAttackDist() + OwnerCharacter->GetAttackComponent()->GetBaseAttackRadius()) + 20.0f;
	ActivateSkillRange_Target(Skill18_LimitDistance, Skill18_LimitDistance, ESkillRangeType::Square);
	if(SkillRange_Target.IsValid()) 
    {
        SkillRange_Target->SetMaxDist(0.0f); // 최대 사거리 0.5배 해줘야지 같은 거리로 뒤로 밀림
	    SkillRange_Target->SetSkill(SkillAbilityNestingData);	
        SkillRange_Target->SetMouseControll(false);
    }
}

void USkillNumber18::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
    OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_18_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    Activate_Skill();
}

void USkillNumber18::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TWeakObjectPtr<ASpinSlash> SpinSlash = GetWorld()->SpawnActor<ASpinSlash>(OwnerCharacter->GetAPSkillHubComponent()->GetSpinSlash(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!SpinSlash.IsValid()) return; 
	SpinSlash->SetOwner(OwnerCharacter.Get());
    SpinSlash->SetSlashWidth(Skill18_LimitDistance);
    SpinSlash->SetSkill(SkillAbilityNestingData);	

    SpinSlash->OnCharging();
}

void USkillNumber18::SkillEnd()
{
    if(!OwnerCharacter.IsValid()) return;
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->StopSkill_18_Montage();

    Remove_Skill();
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
    OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber18::UpdateSkillData()
{
}
