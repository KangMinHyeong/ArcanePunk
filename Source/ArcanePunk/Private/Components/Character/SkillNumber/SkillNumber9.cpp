

#include "Components/Character/SkillNumber/SkillNumber9.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneMine.h"

void USkillNumber9::BeginPlay()
{
	Super::BeginPlay();
	SkillAbilityNestingData.SkillName = TEXT("Skill_9");
}

void USkillNumber9::AddAbilityList()
{
    // EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber9::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

    OwnerCharacter->SetDoing(true);
    SetAbility(WhichKey);
    SkillKey = WhichKey;
    Skilling = true;
    CurrentSkillType = SkillType;
    OnSkill();
    
}

void USkillNumber9::OnSkill()
{
	Super::OnSkill();
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_9_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber9::Activate_Skill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneMine = GetWorld()->SpawnActor<AArcaneMine>(OwnerCharacter->GetArcaneMineClass(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation() + FRotator(-45,0,0));
	if(!ArcaneMine.IsValid()) return; 
    ArcaneMine->SetOwner(OwnerCharacter);
	ArcaneMine->SetSkill(SkillAbilityNestingData);	
 
}
