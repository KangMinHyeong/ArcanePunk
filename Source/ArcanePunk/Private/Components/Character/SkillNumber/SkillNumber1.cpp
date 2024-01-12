
#include "Components/Character/SkillNumber/SkillNumber1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Skill/SwordImpact.h"

void USkillNumber1::PlaySkill(uint8 SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->SetDoing(true);
	
	if(SkillType == 2)
	{
		if(CheckSmartKey(SkillType, OwnerCharacter))
		{
			OnSkill(SkillType);
		}
		else
		{
			auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
			SetMouseCursor(PC, ECursorType::Crosshairs);
			PC->DisplayHomingUI(1, SkillType);
		}
	}
	else
	{
		OnSkill(SkillType);
	}
}

void USkillNumber1::OnSkill(uint8 SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	CurrentSkillType = SkillType;

	OwnerAnim->PlaySkill_1_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber1::Activate_Skill1()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordSkill = GetWorld()->SpawnActor<ASwordImpact>(OwnerCharacter->GetSwordImpactClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*35.0f, OwnerCharacter->GetActorRotation(), SpawnParams);
	if(SwordSkill) SwordSkill->SetSkillType(CurrentSkillType);
}