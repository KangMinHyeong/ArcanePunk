// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/SkillNumber/SkillNumber8.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneBall.h"

USkillNumber8::USkillNumber8()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_8");
}

void USkillNumber8::BeginPlay()
{
	Super::BeginPlay();

	Skill8_LimitDistance = Skill8_LimitDistance_Origin;
	Skill8_Wide = Skill8_Wide_Origin;
}

void USkillNumber8::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber8::PlaySkill()
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
        Spawn_SkillRange();
	}
}

void USkillNumber8::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

    // if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_8);}
	
	ActivateSkillRange_Target(Skill8_Wide, Skill8_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill8_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		USkillDataManager::CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber8::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_8_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
    CharacterRotation();
}

void USkillNumber8::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    SpawnAddLocation = Skill8_Wide;

	ArcaneBall = GetWorld()->SpawnActor<AArcaneBall>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneBallClass(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation());
	if(!ArcaneBall.IsValid()) return; 
    ArcaneBall->SetOwner(OwnerCharacter.Get());
    ArcaneBall->SetBallRadius(Skill8_Wide);
	float Size = ArcaneBall->GetActorScale3D().Y/ ArcaneBall->DefaultSize;
    ArcaneBall->SetDeadTime((SkillRange_Target->GetTargetDistance() * 2.0f - SpawnAddLocation) / ArcaneBall->GetBallSpeed()); // 거리 = 속력 * 시간 으로 DeadTime 수정
	ArcaneBall->SetSkill(SkillAbilityNestingData, this);	

	Remove_Skill();

    OwnerCharacter->SetDoing(false);
}

void USkillNumber8::SkillEnd()
{
	Super::SkillEnd();
    bActivate = false;
}

void USkillNumber8::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	
	float Dist = Skill8_LimitDistance_Origin;
	float Wide = Skill8_Wide_Origin;
	float Cool = SkillNameListData.CoolTime;
	for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        if(It.Key == 2) {
		UpdatAbilityData(EEnHanceType::Silver, It.Key); 
		OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Dist, AbilityData->Coefficient_X, It.Value);
		} // 사거리 강화}
    }
    // for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    // {
    // }
    // for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    // {
    // }
	Skill8_LimitDistance = Dist;
	Skill8_Wide = Wide;
	CurrentCoolTime = Cool;
}
