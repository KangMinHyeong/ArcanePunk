
#include "Components/Character/SkillNumber/UltSkillNumber_2.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Common/APGhostTrailSpawnComponent.h"
#include "GameElements/GhostTrail/APGhostTrail.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Skill/TimeRewinder.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_2::UUltSkillNumber_2()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_2");
}

void UUltSkillNumber_2::BeginPlay()
{
    Super::BeginPlay();
    OnCoolDown();
}

void UUltSkillNumber_2::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(!CheckSkillCondition()) return;

	Skilling = true;
	OwnerCharacter->SetDoing(true);
	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

	OnSkill();
}

void UUltSkillNumber_2::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return;
    
	auto SkillTrail = OwnerCharacter->GetGhostTrailSpawnComp()->GetSkillTrail(); if(!SkillTrail) return;

	ActivateSkillRange_Round(Ult2_TargetRange);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	
	
	OwnerCharacter->GetGhostTrailSpawnComp()->CheckRelease(true);

	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
}

void UUltSkillNumber_2::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;
	bActivate = true;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);

	OwnerCharacter->GetGhostTrailSpawnComp()->CheckRelease(false);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TimeRewinder = GetWorld()->SpawnActor<ATimeRewinder>(OwnerCharacter->GetAPSkillHubComponent()->GetTimeRewinderClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!TimeRewinder.IsValid()) return;
	// TimeRewinder->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	TimeRewinder->SetOwner(OwnerCharacter.Get());
	TimeRewinder->SetRewinderWidth(Ult2_TargetRange);
	TimeRewinder->SetRewinderLocation(SkillRange_Circle->GetActorLocation());
	TimeRewinder->SetSkill(SkillAbilityNestingData, this);	

	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
    // ArcaneRain = GetWorld()->SpawnActor<AArcaneRain>(OwnerCharacter->GetArcaneRainClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	// if(!ArcaneRain.IsValid()) return;
	// // ArcaneRain->SetSkill(CurrentSkillType, CurrentSkillAbility);	
	// ArcaneRain->SetOwner(OwnerCharacter.Get());
	// ArcaneRain->SetRainEffect();
}

void UUltSkillNumber_2::SkillEnd()
{
	Remove_Skill(); OwnerCharacter->SetDoing(false);
}

void UUltSkillNumber_2::UpdateSkillData()
{
}

void UUltSkillNumber_2::OnCoolDown()
{
    // To Do : 분신 소환
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetGhostTrailSpawnComp()->SetSkillTrail(true);
	bActivate = false;
}