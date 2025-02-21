
#include "Components/Character/SkillNumber/UltSkillNumber_11.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/Overload.h"

UUltSkillNumber_11::UUltSkillNumber_11()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_11");
}

void UUltSkillNumber_11::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_11::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;

		OnSkill();
	}
}

void UUltSkillNumber_11::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_11_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    Activate_UltSkill_11();
}

void UUltSkillNumber_11::Activate_UltSkill_11()
{
    if(!OwnerCharacter.IsValid()) return;

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Overload = GetWorld()->SpawnActor<AOverload>(OwnerCharacter->GetAPSkillHubComponent()->GetOverload(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!Overload.IsValid()) return; 
	Overload->SetOwner(OwnerCharacter.Get());
    Overload->SetRange(UltSkill11_LimitDistance);
    Overload->SetSkill(SkillAbilityNestingData, this);	
}

void UUltSkillNumber_11::Enhance()
{
    if(!OwnerCharacter.IsValid() || !Overload.IsValid()) return;

    Overload->OnCharging();
}

void UUltSkillNumber_11::Activate_Skill()
{
	if(!OwnerCharacter.IsValid() || !Overload.IsValid()) return;

    Overload->StartFire();
}

void UUltSkillNumber_11::SkillEnd()
{
    if(!OwnerCharacter.IsValid()) return;
	Remove_Skill();
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_11::UpdateSkillData()
{
}