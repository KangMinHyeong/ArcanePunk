
#include "Components/Character/SkillNumber/SkillNumber4.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneBomb.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"

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
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	EnableSkillAbilityList.Add(ESkillAbility::Homing);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber4::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);
		SetAbility(WhichKey);
		SkillKey = WhichKey;
		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill4();
	}


}

void USkillNumber4::Spawn_Skill4()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	if(CurrentSkillAbility.Contains(ESkillAbility::Homing))
	{
		if(CheckSmartKey(SkillKey, OwnerCharacter))
		{
			OnSkill();
		}
		else
		{
			auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
			SetMouseCursor(PC, ESkillCursor::Crosshairs);
			PC->DisplayHomingUI(ESkillNumber::Skill_4);
		}
	}
	else
	{
		OwnerCharacterPC->bShowMouseCursor = false;
		CursorImmediately();

		if(!CheckSmartKey(SkillKey, OwnerCharacter)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_4);}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ActivateSkillRange_Target(Skill4_TargetRange, Skill4_TargetRange, true);
		if(SkillRange_Target) SkillRange_Target->SetMaxDist(Skill4_LimitDistance);
		if(SkillRange_Target) SkillRange_Target->SetSkill(CurrentSkillType, CurrentSkillAbility);

		ActivateSkillRange_Circle(Skill4_LimitDistance);
		if(SkillRange_Circle) SkillRange_Circle->SetSkill(CurrentSkillType, CurrentSkillAbility);

		OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
		OwnerCharacter->SetDoing(false);
		SetComponentTickEnabled(true);
	}
}

void USkillNumber4::OnSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_4_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	CharacterRotation();
	
	Remove_Skill();
}

void USkillNumber4::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber4::Activate_Skill4()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto ArcaneBomb = GetWorld()->SpawnActor<AArcaneBomb>(OwnerCharacter->GetArcaneBombClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, FRotator::ZeroRotator, SpawnParams);
	if(ArcaneBomb)
	{
		ArcaneBomb->SetOwner(OwnerCharacter);
		ArcaneBomb->SetSkill(CurrentSkillType, CurrentSkillAbility);
		if(SkillRange_Target) ArcaneBomb->SetTargetPoint(Skill4_TargetRange, SkillRange_Target->GetActorLocation());
	}
}
