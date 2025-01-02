
#include "Components/Character/SkillNumber/SkillNumber4.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneBomb.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber4::USkillNumber4()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_4");
}

void USkillNumber4::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber4::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillNumber4::AddAbilityList()
{
}

void USkillNumber4::PlaySkill()
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

void USkillNumber4::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	
	OwnerCharacterPC->bShowMouseCursor = false;
		CursorImmediately();

		// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_4);}

		ActivateSkillRange_Target(Skill4_TargetRange, Skill4_TargetRange, ESkillRangeType::Control_Circle);
		if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill4_LimitDistance);
		if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

		ActivateSkillRange_Round(Skill4_LimitDistance);
		if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

		// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
		OwnerCharacter->SetDoing(false);
		SetComponentTickEnabled(true);
}

void USkillNumber4::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_4_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	CharacterRotation();
	
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
}

void USkillNumber4::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber4::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto ArcaneBomb = GetWorld()->SpawnActor<AArcaneBomb>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneBombClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, FRotator::ZeroRotator, SpawnParams);
	if(!ArcaneBomb) return;	
	ArcaneBomb->SetOwner(OwnerCharacter.Get());
	ArcaneBomb->SetSkill(SkillAbilityNestingData, this);	
	if(SkillRange_Target.IsValid()) ArcaneBomb->SetTargetPoint(Skill4_TargetRange, SkillRange_Target->GetActorLocation());
	
	Remove_Skill();
}

void USkillNumber4::UpdateSkillData()
{
}