
#include "Components/Character/SkillNumber/SkillNumber20.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneTurret.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneCannon.h"

USkillNumber20::USkillNumber20()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_20");

	MaxChargeNum_Origin = 3;

    Skill20_Wide = 40.0f;
    Skill20_LimitDistance = 425.0f;
}

void USkillNumber20::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber20::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_SkillRange();
	}
}

void USkillNumber20::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    // if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_20);}
	
	ActivateSkillRange_Target(Skill20_Wide, Skill20_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill20_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	}

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber20::OnSkill()
{
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);

    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_20_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    CharacterRotation();
    SpawnRotation = SkillRange_Target->GetActorRotation();
}

void USkillNumber20::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;
	CurrentChargeNum--;
	OwnerCharacter->GetAPHUD()->OnChargeTime.Broadcast(SkillKey);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneCannon = GetWorld()->SpawnActor<AArcaneCannon>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneCannon(), OwnerCharacter->GetActorLocation(), SpawnRotation, SpawnParams);
    if(!ArcaneCannon) return;
	ArcaneCannon->SetOwner(OwnerCharacter.Get());
    ArcaneCannon->SetMaxCannonDist(Skill20_LimitDistance * 2.0f);
    ArcaneCannon->SetCannonWidth(Skill20_Wide);
	ArcaneCannon->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
	Remove_Skill();
}

void USkillNumber20::UpdateSkillData()
{
}

bool USkillNumber20::CheckSkillCondition()
{
	bool Check = Super::CheckSkillCondition();
	if(CurrentChargeNum == 0) {OwnerCharacterPC->DisplayNotEnoughMPUI(); Check = false;}

    return Check;
}
