
#include "Components/Character/SkillNumber/UltSkillNumber_12.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneReflector.h"

UUltSkillNumber_12::UUltSkillNumber_12()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_12");
}

void UUltSkillNumber_12::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_12::PlaySkill()
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
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;

		OnSkill();
	}
}

void UUltSkillNumber_12::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
    OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_12_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    Activate_UltSkill_12();
}

void UUltSkillNumber_12::Activate_UltSkill_12()
{
    if(!OwnerCharacter.IsValid()) return;

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneReflector = GetWorld()->SpawnActor<AArcaneReflector>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneReflector(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!ArcaneReflector.IsValid()) return; 
	ArcaneReflector->SetOwner(OwnerCharacter.Get());
    ArcaneReflector->SetDotRange(UltSkill12_LimitDistance);
    ArcaneReflector->SetSkill(SkillAbilityNestingData, this);	
}

void UUltSkillNumber_12::Enhance()
{
    if(!OwnerCharacter.IsValid() || !ArcaneReflector.IsValid()) return;

    ArcaneReflector->OnCharging();
}

void UUltSkillNumber_12::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;
    Remove_Skill();
    OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
    bActivate = true;
}

void UUltSkillNumber_12::SkillEnd()
{
    if(!OwnerCharacter.IsValid()) return;
    bActivate = false;
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
    OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void UUltSkillNumber_12::UpdateSkillData()
{
}
