#include "Components/Character/APAnimHubComponent.h"

#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APSkillHubComponent.h"

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
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	// MontageEnd
	OwnerAnim->OnMontageEnded.AddDynamic(this, &UAPAnimHubComponent::PlayerMontageEnded);
}

void UAPAnimHubComponent::BindComboCheck()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	//ComboAttack
	OwnerAnim->OnComboCheck.AddLambda([OwnerCharacter]()->void {
		OwnerCharacter->GetAttackComponent()->ComboCheck();
	});
}

void UAPAnimHubComponent::PlayerMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
	
	if(Montage == OwnerAnim->Attack_A_Montage) OnAttack_A_MontageEnded();
	else if(Montage == OwnerAnim->Attack_B_Montage) OnAttack_B_MontageEnded();
	else if(Montage == OwnerAnim->Skill_1_Montage) OnSkill_1_MontageEnded();
	else if(Montage == OwnerAnim->Skill_2_Montage) OnSkill_2_MontageEnded();
	else if(Montage == OwnerAnim->Skill_3_Montage) OnSkill_3_MontageEnded();
	else if(Montage == OwnerAnim->Skill_4_Montage) OnSkill_4_MontageEnded();
	else if(Montage == OwnerAnim->Skill_5_Montage) OnSkill_5_MontageEnded();
	else if(Montage == OwnerAnim->Skill_6_Montage) OnSkill_6_MontageEnded();
	else if(Montage == OwnerAnim->Skill_7_Montage) OnSkill_7_MontageEnded();
	else if(Montage == OwnerAnim->Skill_8_Montage) OnSkill_8_MontageEnded();
	else if(Montage == OwnerAnim->Skill_9_Montage) OnSkill_9_MontageEnded();
	else if(Montage == OwnerAnim->Skill_10_Montage) OnSkill_10_MontageEnded();
	else if(Montage == OwnerAnim->Skill_11_Montage) OnSkill_11_MontageEnded();
	else if(Montage == OwnerAnim->Skill_12_Montage) OnSkill_12_MontageEnded();
	else if(Montage == OwnerAnim->Skill_13_Montage) OnSkill_13_MontageEnded();
	else if(Montage == OwnerAnim->Skill_14_Montage) OnSkill_14_MontageEnded();
	else if(Montage == OwnerAnim->UltSkill_1_Montage) OnUltSkill_1_MontageEnded();
}

void UAPAnimHubComponent::OnAttack_A_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerCharacter->GetAttackComponent()->SetAttack_A(false);
	OwnerCharacter->GetAttackComponent()->ComboAttackEnd();
	OwnerCharacter->SetCanMove(true);
	OwnerCharacter->SetDoing(false);
}

void UAPAnimHubComponent::OnAttack_B_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAttackComponent()->SetAttack_B(false);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_1_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_2_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_3_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_4_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_5_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	GetWorld()->GetTimerManager().ClearTimer(OwnerAnim->Skill5_FireTimerHandle);
	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->Remove_Skill();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnSkill_6_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber6()->Skill6_End();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

void UAPAnimHubComponent::OnSkill_7_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_8_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	// OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_9_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_10_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_11_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_12_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_13_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	// OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnSkill_14_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UAPAnimHubComponent::OnUltSkill_1_MontageEnded()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetUltSkillNumber1()->UltSkill1_End();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	
}

