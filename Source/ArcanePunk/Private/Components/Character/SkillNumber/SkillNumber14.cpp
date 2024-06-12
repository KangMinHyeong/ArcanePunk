// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber14.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/SwordClutch.h"

USkillNumber14::USkillNumber14()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_14");
}

void USkillNumber14::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber14::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
}

void USkillNumber14::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
        if(bActivate)
        {
            // if(ArcaneBall) ArcaneBall->Explosion();
			// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
        }
        else
        {
			if(!CheckSkillCondition()) return;
            OwnerCharacter->SetDoing(true);
            Skilling = true;
            Spawn_SkillRange();
        }
	}
}

void USkillNumber14::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = true;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_14);}

	ActivateSkillRange_Target(Skill14_LimitDistance, Skill14_LimitDistance, ESkillRangeType::Around_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill14_LimitDistance+Skill14_Wide/2);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetWide(Skill14_Wide);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(Skill14_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber14::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlaySkill_14_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
}

void USkillNumber14::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;
	
    OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto SwordClutch = GetWorld()->SpawnActor<ASwordClutch>(OwnerCharacter->GetAPSkillHubComponent()->GetSwordClutchClass(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!SwordClutch) return;
	SwordClutch->SetOwner(OwnerCharacter.Get());
	if(SkillRange_Target.IsValid()) SwordClutch->SetDistance(SkillRange_Target->GetTargetDistance() - Skill14_Wide/2);
	if(SkillRange_Target.IsValid()) SwordClutch->SetClutchSpeed(SkillRange_Target->GetMaxDist() / SkillRange_Target->GetTargetDistance());
	SwordClutch->SetWide(Skill14_Wide);
	SwordClutch->AddDestroyTime(AddDuration);
	SwordClutch->SetSkill(SkillAbilityNestingData, this);	
	
	bActivate = true;
	Remove_Skill();
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
}

void USkillNumber14::SkillEnd()
{
	Super::SkillEnd();
	bActivate = false; 	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void USkillNumber14::UpdateSkillData()
{
}
