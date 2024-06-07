// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber13.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneTent.h"

USkillNumber13::USkillNumber13()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_13");
}

void USkillNumber13::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber13::AddAbilityList()
{
    // EnableSkillAbilityList.Add(ESkillAbility::Gigant);
}

void USkillNumber13::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void USkillNumber13::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlaySkill_13_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber13::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;
	
    OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneTent = GetWorld()->SpawnActor<AArcaneTent>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneTentClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneTent) return;
	ArcaneTent->SetOwner(OwnerCharacter.Get());
	ArcaneTent->SetSkill(SkillAbilityNestingData, this);	
	
	Remove_Skill();
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
}

void USkillNumber13::SkillEnd()
{
	Super::SkillEnd();
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber13::UpdateSkillData()
{
}