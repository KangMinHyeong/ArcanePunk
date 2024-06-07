
#include "Components/Character/SkillNumber/UltSkillNumber_13.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneWave.h"

UUltSkillNumber_13::UUltSkillNumber_13()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_13");

}

void UUltSkillNumber_13::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_13::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
    if(!CheckSkillCondition()) return;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	Spawn_SkillRange();   
    OnSkill();
}

void UUltSkillNumber_13::Spawn_SkillRange()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	ActivateSkillRange_Round(UltSkill13_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	
}

void UUltSkillNumber_13::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
    OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_13_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
}

void UUltSkillNumber_13::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ArcaneWave = GetWorld()->SpawnActor<AArcaneWave>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneWave(), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), SpawnParams);
    if(!ArcaneWave.IsValid()) return;
	ArcaneWave->SetOwner(OwnerCharacter.Get());
    ArcaneWave->SetMaxRadius(UltSkill13_LimitDistance);
	ArcaneWave->SetSkill(SkillAbilityNestingData, this);	

}

void UUltSkillNumber_13::SkillEnd()
{
	Remove_Skill();
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void UUltSkillNumber_13::UpdateSkillData()
{

}
