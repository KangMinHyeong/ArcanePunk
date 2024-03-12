
#include "Components/Character/SkillNumber/SkillNumber12.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneRage.h"

void USkillNumber12::AddAbilityList()
{
}

void USkillNumber12::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
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

void USkillNumber12::OnSkill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_12_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber12::Activate_Skill()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneRage = GetWorld()->SpawnActor<AArcaneRage>(OwnerCharacter->GetArcaneRageClass(), OwnerCharacter->GetActorLocation() + FVector(0,0,SpawnAddLocation), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneRage) return;
	ArcaneRage->SetOwner(OwnerCharacter);
	ArcaneRage->SetSkill(CurrentSkillType, CurrentSkillAbility);
    ArcaneRage->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules ::KeepWorldTransform);
}
