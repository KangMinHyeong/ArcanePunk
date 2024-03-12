
#include "Components/Character/SkillNumber/SkillNumber6.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/Shouting.h"

void USkillNumber6::BeginPlay()
{
	Super::BeginPlay();

}

void USkillNumber6::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	
}

void USkillNumber6::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->SetDoing(true);
    SetAbility(WhichKey);

	CurrentSkillType = SkillType;
	
	OnSkill();

	// if(CurrentSkillAbility.Contains(ESkillAbility::Homing))
	// {
	// 	if(CheckSmartKey(WhichKey, OwnerCharacter))
	// 	{
	// 		OnSkill(SkillType);
	// 	}
	// 	else
	// 	{
	// 		auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	// 		SetMouseCursor(PC, ESkillCursor::Crosshairs);
	// 		PC->DisplayHomingUI(ESkillNumber::Skill_4, SkillType);
	// 	}
	// }
	// else
	// {
		
	// }
}

void USkillNumber6::OnSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_6_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber6::Remove_Skill()
{
}

void USkillNumber6::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Shouting = GetWorld()->SpawnActor<AShouting>(OwnerCharacter->GetShoutingClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!Shouting.IsValid()) return;
	Shouting->SetSkill(CurrentSkillType, CurrentSkillAbility);	
	Shouting->SetOwner(OwnerCharacter);
	Shouting->SetShoutingEffect();
}

void USkillNumber6::SkillEnd()
{
	if(Shouting.IsValid()) Shouting->DestroySKill();
}
