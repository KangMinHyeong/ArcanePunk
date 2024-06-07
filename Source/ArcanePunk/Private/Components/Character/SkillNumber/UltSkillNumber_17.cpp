
#include "Components/Character/SkillNumber/UltSkillNumber_17.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneArea.h"

UUltSkillNumber_17::UUltSkillNumber_17()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_17");
}

void UUltSkillNumber_17::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_17::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void UUltSkillNumber_17::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);

    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_17_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_17::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneArea = GetWorld()->SpawnActor<AArcaneArea>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneArea(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!ArcaneArea) return;
	ArcaneArea->SetOwner(OwnerCharacter.Get());
	ArcaneArea->SetSkill(SkillAbilityNestingData, this);	

    Remove_Skill();
}

void UUltSkillNumber_17::SkillEnd()
{
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void UUltSkillNumber_17::UpdateSkillData()
{

}
