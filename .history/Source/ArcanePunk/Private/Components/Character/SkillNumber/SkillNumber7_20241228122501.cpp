
#include "Components/Character/SkillNumber/SkillNumber7.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneCutter.h"

USkillNumber7::USkillNumber7()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_7");

}

void USkillNumber7::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber7::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber7::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling) {Remove_SkillRange();}
	else
	{
		if(!CheckSkillCondition()) return;
		Skilling = true;
		Spawn_SkillRange();
	}
}

void USkillNumber7::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	OwnerCharacterPC->bShowMouseCursor = false; USkillDataManager::CursorImmediately();

	ActivateSkillRange_Target(Skill7_Wide, Skill7_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill7_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	
	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		USkillDataManager::CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber7::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_7_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
    CharacterRotation();
}

void USkillNumber7::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber7::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneCutter = GetWorld()->SpawnActor<AArcaneCutter>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneCutterClass(), OwnerCharacter->GetMesh()->GetComponentLocation() + OwnerCharacter->GetActorUpVector()*151.0f , OwnerCharacter->GetActorRotation()); // 
	if(!ArcaneCutter.IsValid()) return; 
	ArcaneCutter->SetOwner(OwnerCharacter.Get());
    ArcaneCutter->SetDist((Skill7_LimitDistance * 2.0f - ArcaneCutter->GetTriggerWide() * 2.0f )); // 거리 = 속력 * 시간 으로 DeadTime 수정
	ArcaneCutter->SetCutterWide(Skill7_Wide);	
	ArcaneCutter->SetSkill(SkillAbilityNestingData, this);	

	Remove_Skill();	
	GetWorld()->GetTimerManager().SetTimer(Skill7_TimerHandle, this, &USkillNumber7::CheckDoubleCutter, DoubleCutDelay, false);
}

void USkillNumber7::CheckDoubleCutter()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill7_TimerHandle);
	if(bDoubleCutter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter.Get();
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto DoubleCutter = GetWorld()->SpawnActor<AArcaneCutter>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneCutterClass(), OwnerCharacter->GetMesh()->GetComponentLocation() + OwnerCharacter->GetActorUpVector()*150.0f , OwnerCharacter->GetActorRotation()); // 
		if(!DoubleCutter) return; 
		DoubleCutter->SetOwner(OwnerCharacter.Get());
		DoubleCutter->SetDist((Skill7_LimitDistance * 2.0f - DoubleCutter->GetTriggerWide() * 2.0f )); // 거리 = 속력 * 시간 으로 DeadTime 수정
		DoubleCutter->SetSkill(SkillAbilityNestingData, this);	
		DoubleCutter->SetCutterWide(Skill7_Wide);	
		DoubleCutter->SetDamagedCoeff(DoubleCutterDamaged);	
	}
}

void USkillNumber7::SkillEnd()
{
	Super::SkillEnd();
}

void USkillNumber7::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	float Dist = Skill7_LimitDistance_Origin;
	float Wide = Skill7_Wide_Origin;
	float Cool = SkillNameListData.CoolTime;
	float Damaged = DoubleCutterDamaged_Origin;
	for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
		if(It.Key == 2) {
			UpdatAbilityData(EEnHanceType::Silver, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Dist, AbilityData->Coefficient_X, It.Value); // 사거리 강화}
		} 
	}
    for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
		if(It.Key == 3) {
			UpdatAbilityData(EEnHanceType::Gold, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Wide, AbilityData->Coefficient_X, It.Value); // 가로 강화}
		}
    }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
		if(It.Key == 3) {
			bDoubleCutter = true;
			UpdatAbilityData(EEnHanceType::Platinum, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Damaged, AbilityData->Coefficient_X, It.Value); // 가로 강화}
		}
    }
	Skill7_LimitDistance = Dist;
	Skill7_Wide = Wide;
	CurrentCoolTime = Cool;
	DoubleCutterDamaged = Damaged;
}
