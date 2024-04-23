
#include "Components/Character/SkillNumber/SkillNumber5.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneBeam.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

USkillNumber5::USkillNumber5()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_5");
}

void USkillNumber5::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber5::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
		OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_Skill5();

		OnSkill();
	}
}

void USkillNumber5::Spawn_Skill5()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	ActivateSkillRange_Target(Skill5_Wide, Skill5_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill5_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber5::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_5_Montage();
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	Activate_Skill_5();
}

void USkillNumber5::Activate_Skill_5()
{
	if(!OwnerCharacter.IsValid()) return;
	// RemoveEffect();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneBeam = GetWorld()->SpawnActor<AArcaneBeam>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneBeamClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!ArcaneBeam.IsValid()) return; 
	ArcaneBeam->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BeamPoint"));
	ArcaneBeam->SetOwner(OwnerCharacter.Get());
	ArcaneBeam->SetDistance(Skill5_LimitDistance * 2.0f - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
	ArcaneBeam->SetWide(Skill5_Wide);
	ArcaneBeam->SetSkill(SkillAbilityNestingData);	

}

void USkillNumber5::Activate_Skill()
{
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;
	
	ArcaneBeam->SetBeamEffect();	
}

void USkillNumber5::Remove_Skill()
{
	Super::Remove_Skill();
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber5::SkillEnd()
{
	Remove_Skill();
	if(ArcaneBeam.IsValid()) ArcaneBeam->DestroySKill();
}

void USkillNumber5::Enhance()
{
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;

	ArcaneBeam->OnCharging();
}

void USkillNumber5::DoubleEnhance()
{
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;
	ArcaneBeam->OnChargingEnhance();
}

void USkillNumber5::UpdateSkillData()
{
}
