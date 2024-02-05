
#include "Components/Character/SkillNumber/SkillNumber10.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneTurret.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"

void USkillNumber10::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber10::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);
		SetAbility(WhichKey);
        SkillKey = WhichKey;
		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill10();
	}
}

void USkillNumber10::Spawn_Skill10()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	if(!CheckSmartKey(SkillKey, OwnerCharacter)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_10);}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ActivateSkillRange_Target(Skill10_TargetRange, Skill10_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target) SkillRange_Target->SetMaxDist(Skill10_LimitDistance);
	if(SkillRange_Target) SkillRange_Target->SetSkill(CurrentSkillType, CurrentSkillAbility);

	ActivateSkillRange_Round(Skill10_LimitDistance);
	if(SkillRange_Circle) SkillRange_Circle->SetSkill(CurrentSkillType, CurrentSkillAbility);

	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber10::OnSkill()
{
    SetComponentTickEnabled(false);
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_10_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    CharacterRotation();

    if(SkillRange_Target) SkillRange_TargetLocation = SkillRange_Target->GetActorLocation();
    Remove_Skill();
}

void USkillNumber10::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber10::Activate_Skill10()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneTurret = GetWorld()->SpawnActor<AArcaneTurret>(OwnerCharacter->GetArcaneTurretClass(), SkillRange_TargetLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneTurret) return;
	ArcaneTurret->SetOwner(OwnerCharacter);
	ArcaneTurret->SetSkill(CurrentSkillType, CurrentSkillAbility);

    
}

