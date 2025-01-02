

#include "Components/Character/SkillNumber/SkillNumber9.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneMine.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameInstance/APGameInstance.h"

USkillNumber9::USkillNumber9()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_9");

	MaxChargeNum_Origin = 3;
}

void USkillNumber9::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber9::AddAbilityList()
{
    // EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber9::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
        if(bActivate)
        {
            if(!ArcaneMine.IsValid()) return;
			if(ArcaneMine->IsExploaionEnable())
			{
				ArcaneMine->Explosion();
			} 
        }
        else
        {
			if(!CheckSkillCondition()) return;
            OwnerCharacter->SetDoing(true);
			Skilling = true;
			Spawn_SkillRange();
        }
		
		// OnSkill();
	}
    
}

void USkillNumber9::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_9);}

	ActivateSkillRange_Target(Skill9_TargetRange, Skill9_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill9_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(Skill9_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);

}

void USkillNumber9::OnSkill()
{
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
    OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_9_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);

	SpawnLocation = SkillRange_Target->GetActorLocation();
    CharacterRotation();

}

void USkillNumber9::Activate_Skill()
{
	Super::Activate_Skill();
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	CurrentChargeNum--;
	OwnerCharacter->GetAPHUD()->OnChargeTime.Broadcast(SkillKey);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneMine = GetWorld()->SpawnActor<AArcaneMine>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneMineClass(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation());
	if(!ArcaneMine.IsValid()) return; 
    ArcaneMine->SetOwner(OwnerCharacter.Get());
	ArcaneMine->SetExplosionRadius(Skill9_TargetRange);
	ArcaneMine->SetTarget(SpawnLocation);
	ArcaneMine->SetSkill(SkillAbilityNestingData, this);	

	bActivate = true;
	Remove_Skill();
}

void USkillNumber9::SkillEnd()
{
	Super::SkillEnd();
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
}

void USkillNumber9::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	float Dist = Skill9_LimitDistance_Origin;
	float Wide = Skill9_TargetRange_Origin;
	float Cool = SkillNameListData.CoolTime;
	int32 Charge = MaxChargeNum_Origin;
	for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        if(It.Key == 1){UpdatAbilityData(EEnHanceType::Silver, It.Key); OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Dist, AbilityData->Coefficient_X, It.Value); }// 사거리 강화}
		if(It.Key == 2) {UpdatAbilityData(EEnHanceType::Silver, It.Key); OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Wide, AbilityData->Coefficient_X, It.Value); }// 사이즈 강화}
    }
    // for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    // {
    // }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
		if(It.Key == 2)
		{
			UpdatAbilityData(EEnHanceType::Platinum, It.Key);
			OwnerCharacter->GetAPSkillAbility()->Coefficient_Add(Charge, AbilityData->Coefficient_X, It.Value); 
			MaxChargeNum = Charge;
			OwnerCharacter->GetAPHUD()->OnChargeTime.Broadcast(SkillKey);
		}// 충전횟수 증가
	}
	Skill9_LimitDistance = Dist;
	Skill9_TargetRange = Wide;
	CurrentCoolTime = Cool;
}

bool USkillNumber9::CheckSkillCondition()
{
	bool Check = Super::CheckSkillCondition();
	if(CurrentChargeNum == 0) 
	{
		UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::CannotSkill, true, true); 
		Check = false;
	}

    return Check;
}
