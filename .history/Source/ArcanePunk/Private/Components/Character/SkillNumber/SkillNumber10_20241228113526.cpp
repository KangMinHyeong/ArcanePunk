
#include "Components/Character/SkillNumber/SkillNumber10.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneTurret.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameInstance/APGameInstance.h"

USkillNumber10::USkillNumber10()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_10");

	MaxChargeNum_Origin = 3;
}

void USkillNumber10::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber10::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber10::PlaySkill()
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

void USkillNumber10::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_10);}

	ActivateSkillRange_Target(Skill10_TargetRange, Skill10_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill10_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(Skill10_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber10::OnSkill()
{
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);

	OwnerAnim->PlaySkill_10_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    CharacterRotation();

    SpawnLocation = SkillRange_Target->GetActorLocation();
}

void USkillNumber10::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;
	CurrentChargeNum--;
	OwnerCharacter->GetAPHUD()->OnChargeTime.Broadcast(SkillKey);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneTurret = GetWorld()->SpawnActor<AArcaneTurret>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneTurretClass(), SpawnLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneTurret) return;
	ArcaneTurret->SetOwner(OwnerCharacter.Get());
	ArcaneTurret->SetSkill(SkillAbilityNestingData, this);	
	OwnerCharacter->SetDoing(false);

	Remove_Skill();
}

void USkillNumber10::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	int32 Charge = MaxChargeNum_Origin;
	for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
		if(It.Key == 1)
		{
			UpdatAbilityData(EEnHanceType::Platinum, It.Key);
			OwnerCharacter->GetAPSkillAbility()->Coefficient_Add(Charge, AbilityData->Coefficient_X, It.Value); 
			MaxChargeNum = Charge;
			OwnerCharacter->GetAPHUD()->OnChargeTime.Broadcast(SkillKey);
		}// 충전횟수 증가
	}
}

bool USkillNumber10::CheckSkillCondition()
{
	bool Check = Super::CheckSkillCondition();
	if(CurrentChargeNum == 0)
	{
		UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::CannotSkill, true, true); 
		Check = false;
	}

    return Check;
}


