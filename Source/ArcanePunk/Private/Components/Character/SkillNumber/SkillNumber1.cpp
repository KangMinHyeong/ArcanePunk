
#include "Components/Character/SkillNumber/SkillNumber1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/SwordImpact.h"

void USkillNumber1::BeginPlay()
{
	Super::BeginPlay();
	SkillAbilityNestingData.SkillName = TEXT("Skill_1");
}

void USkillNumber1::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber1::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->SetDoing(true);
	
	SetAbility(WhichKey);

	SkillKey = WhichKey;
	CurrentSkillType = SkillType;

	// if(CurrentSkillAbility.Contains(ESkillAbility::Homing))
	// {
	// 	if(CheckSmartKey(WhichKey, OwnerCharacter))
	// 	{
	// 		OnSkill();
	// 	}
	// 	else
	// 	{
	// 		auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	// 		SetMouseCursor(PC, ESkillCursor::Crosshairs);
	// 		PC->DisplayHomingUI(ESkillNumber::Skill_1);
	// 	}
	// }
	// else
	// {
	// 	OnSkill();
	// }
	OnSkill();
}

void USkillNumber1::OnSkill()
{
	Super::OnSkill();
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_1_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber1::Remove_Skill()
{
}

void USkillNumber1::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordSkill = GetWorld()->SpawnActor<ASwordImpact>(OwnerCharacter->GetSwordImpactClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	if(!SwordSkill) return;
	SwordSkill->SetSkill(SkillAbilityNestingData);

}