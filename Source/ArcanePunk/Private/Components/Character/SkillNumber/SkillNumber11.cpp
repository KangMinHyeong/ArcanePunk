
#include "Components/Character/SkillNumber/SkillNumber11.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneField.h"

void USkillNumber11::AddAbilityList()
{
    EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber11::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(bActivate) return;

	bActivate = true;
    OwnerCharacter->SetDoing(true);
	SetAbility(WhichKey);
    SkillKey = WhichKey;
	Skilling = true;
	CurrentSkillType = SkillType;
	OnSkill();
}

void USkillNumber11::OnSkill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_11_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber11::Activate_Skill11()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneField = GetWorld()->SpawnActor<AArcaneField>(OwnerCharacter->GetArcaneFieldClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneField) return;
	ArcaneField->SetOwner(OwnerCharacter);
	ArcaneField->SetSkill(CurrentSkillType, CurrentSkillAbility);
    
}
