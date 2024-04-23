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
			if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
            OwnerCharacter->SetDoing(true);
            Skilling = true;
            Spawn_Skill14();
        }
	}
}

void USkillNumber14::Spawn_Skill14()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = true;
	CursorImmediately();

	if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_14);}

	ActivateSkillRange_Target(Skill14_LimitDistance, Skill14_LimitDistance, ESkillRangeType::Around_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill14_LimitDistance+Skill14_Wide/2);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetWide(Skill14_Wide);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	ActivateSkillRange_Round(Skill14_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData);	

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber14::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlaySkill_14_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
}

void USkillNumber14::Activate_Skill()
{
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
	SwordClutch->SetSkill(SkillAbilityNestingData);	
	
	bActivate = true;
	Remove_Skill();
}

void USkillNumber14::SkillEnd()
{
	bActivate = false; 	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber14::UpdateSkillData()
{
}
