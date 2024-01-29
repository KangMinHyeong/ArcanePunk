
#include "Components/Character/SkillNumber/SkillNumber5.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneBeam.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"

void USkillNumber5::BeginPlay()
{
	Super::BeginPlay();

}

void USkillNumber5::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// EnableSkillAbilityList.Add(ESkillAbility::Homing);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber5::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	switch(SkillType)
	{
		case ESkillTypeState::Type_None:
		SkillKey = WhichKey;
		break;
		case ESkillTypeState::Type_Q:
		SkillKey = ESkillKey::Q;
		break;
		case ESkillTypeState::Type_E:
		SkillKey = ESkillKey::E;
		break;
		case ESkillTypeState::Type_R:
		SkillKey = ESkillKey::R;
		break;
	}

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);
		SetAbility(WhichKey);

		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill5();

		OnSkill5();
	}
}

void USkillNumber5::Spawn_Skill5()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	ActivateSkillRange_Target(Skill5_Wide, Skill5_LimitDistance, false);
	if(SkillRange_Target) SkillRange_Target->SetMaxDist(Skill5_LimitDistance);
	if(SkillRange_Target) SkillRange_Target->SetSkill(CurrentSkillType, CurrentSkillAbility);

	if(CheckSmartKey(SkillKey, OwnerCharacter))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber5::OnSkill5()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_5_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber5::OnSkill()
{
}

void USkillNumber5::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber5::Activate_Skill5()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneBeam = GetWorld()->SpawnActor<AArcaneBeam>(OwnerCharacter->GetArcaneBeamClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!ArcaneBeam) return; 
	ArcaneBeam->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BeamPoint"));
	ArcaneBeam->SetSkill(CurrentSkillType, CurrentSkillAbility);	
	ArcaneBeam->SetOwner(OwnerCharacter);
	if(SkillRange_Target) ArcaneBeam->SetDistance(SkillRange_Target->GetTargetDistance() * 2.0f);
	if(SkillRange_Target) ArcaneBeam->SetWide(SkillRange_Target->GetTargetWide());
	ArcaneBeam->SetBeamEffect();
	Remove_Skill();
}

void USkillNumber5::Skill5_End()
{
	if(ArcaneBeam) ArcaneBeam->DestroySKill();
}