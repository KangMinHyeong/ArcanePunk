#include "Components/Character/APAnimHubComponent.h"

#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSpringArmComponent.h"

UAPAnimHubComponent::UAPAnimHubComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPAnimHubComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAPAnimHubComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPAnimHubComponent::BindAttackCheck()
{
	OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim.IsValid()) return;

	// MontageEnd
	OwnerAnim->OnMontageEnded.AddDynamic(this, &UAPAnimHubComponent::PlayerMontageEnded);
}

void UAPAnimHubComponent::BindComboCheck()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	//ComboAttack
	OwnerAnim->OnComboCheck.AddLambda([this]()->void {
		ComboCheck();
	});
}

void UAPAnimHubComponent::ComboCheck()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAttackComponent()->ComboCheck();
}

void UAPAnimHubComponent::PlayerMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
	OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim.IsValid()) return;
	
	if(Montage == OwnerAnim->Combo_Montage) OnCombo_Montage();
	else if(Montage == OwnerAnim->Parrying_Montage) OnParrying_Montage();
	else if(Montage == OwnerAnim->Skill_1_Montage) OnSkill_1_MontageEnded();
	else if(Montage == OwnerAnim->Skill_2_Montage) OnSkill_2_MontageEnded();
	else if(Montage == OwnerAnim->Skill_3_Montage) OnSkill_3_MontageEnded();
	else if(Montage == OwnerAnim->Skill_4_Montage) OnSkill_4_MontageEnded();
	else if(Montage == OwnerAnim->Skill_5_Montage) OnSkill_5_MontageEnded();
	else if(Montage == OwnerAnim->Skill_5_Fire_Montage) OnSkill_5_Fire_MontageEnded();
	else if(Montage == OwnerAnim->Skill_6_Montage) OnSkill_6_MontageEnded();
	else if(Montage == OwnerAnim->Skill_7_Montage) OnSkill_7_MontageEnded();
	else if(Montage == OwnerAnim->Skill_8_Montage) OnSkill_8_MontageEnded();
	else if(Montage == OwnerAnim->Skill_9_Montage) OnSkill_9_MontageEnded();
	else if(Montage == OwnerAnim->Skill_10_Montage) OnSkill_10_MontageEnded();
	else if(Montage == OwnerAnim->Skill_11_Montage) OnSkill_11_MontageEnded();
	else if(Montage == OwnerAnim->Skill_12_Montage) OnSkill_12_MontageEnded();
	else if(Montage == OwnerAnim->Skill_13_Montage) OnSkill_13_MontageEnded();
	else if(Montage == OwnerAnim->Skill_14_Montage) OnSkill_14_MontageEnded();
	else if(Montage == OwnerAnim->Skill_15_Montage) OnSkill_15_MontageEnded();
	else if(Montage == OwnerAnim->Skill_18_Montage) OnSkill_18_MontageEnded();
	else if(Montage == OwnerAnim->Skill_19_Montage) OnSkill_19_MontageEnded();
	else if(Montage == OwnerAnim->Skill_20_Montage) OnSkill_20_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_1_Montage) OnUltSkill_1_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_4_Montage) OnUltSkill_4_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_5_Montage) OnUltSkill_5_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_6_Montage) OnUltSkill_6_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_8_Montage) OnUltSkill_8_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_9_Montage) OnUltSkill_9_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_10_Montage) OnUltSkill_10_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_11_Montage) OnUltSkill_11_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_12_Montage) OnUltSkill_12_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_13_Montage) OnUltSkill_13_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_14_Montage) OnUltSkill_14_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_15_Montage) OnUltSkill_15_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_17_Montage) OnUltSkill_17_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_Fire_Montage) OnUltSkill_Fire_MontageEnded();
}

void UAPAnimHubComponent::OnCombo_Montage()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->GetAttackComponent()->ComboAttackEnd();
	OwnerCharacter->GetAttackComponent()->SetComboAttack(false);
	OwnerCharacter->GetAPMoveComponent()->SetTickMove(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnParrying_Montage()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	auto Player = Cast<AArcanePunkCharacter>(OwnerCharacter.Get()); 
	if(Player)
	{	
		Player->GetAPHUD()->OnStartCoolTime.Broadcast(ESkillKey::Parrying, Player->GetParryingCoolTime());	
	}

	OwnerCharacter->GetAttackComponent()->SetParrying(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
}

void UAPAnimHubComponent::OnSkill_1_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_2_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_3_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_4_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_5_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

    OwnerCharacter->OnSkillRotationTrigger.Broadcast();
	OwnerAnim->PlaySkill_5_Fire_Montage();
}

void UAPAnimHubComponent::OnSkill_5_Fire_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;
	
	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnSkill_6_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnSkill_7_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_8_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_9_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	// OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_10_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_11_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_12_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_13_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_14_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_15_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_18_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_19_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_20_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->OnSkillEndTrigger.Broadcast();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_1_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);

	if(auto OwnerPlayer = Cast<AArcanePunkCharacter>(OwnerCharacter))
	{
		OwnerPlayer->GetRSkillNumber()->SkillEnd();
	}
	
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_4_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	// OwnerCharacter->GetRSkillNumber()->SkillEnd();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_5_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_6_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	// OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_8_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_9_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnUltSkill_10_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_11_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerAnim->OnUltChargeEnd.Broadcast();
}

void UAPAnimHubComponent::OnUltSkill_12_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_13_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	if(auto OwnerPlayer = Cast<AArcanePunkCharacter>(OwnerCharacter))
	{
		OwnerPlayer->GetRSkillNumber()->SkillEnd();
	}
}

void UAPAnimHubComponent::OnUltSkill_14_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_15_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_17_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_Fire_MontageEnded()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerAnim.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

