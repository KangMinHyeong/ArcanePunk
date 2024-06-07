
#include "Components/Character/SkillNumber/SkillNumber1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/SwordImpact.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber1::USkillNumber1()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_1");
}

void USkillNumber1::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber1::AddAbilityList()
{

}

void USkillNumber1::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(!CheckSkillCondition()) return;

	OwnerCharacter->SetDoing(true);

	OnSkill();
}

void USkillNumber1::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_1_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber1::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto SwordSkill = GetWorld()->SpawnActor<ASwordImpact>(OwnerCharacter->GetAPSkillHubComponent()->GetSwordImpactClass(), OwnerCharacter->GetActorLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	if(!SwordSkill) return;
	SwordSkill->SetSkill(SkillAbilityNestingData, this);

	Remove_Skill();
}

void USkillNumber1::Remove_Skill()
{
	Super::Remove_Skill();
	
}

void USkillNumber1::UpdateSkillData()
{
}
