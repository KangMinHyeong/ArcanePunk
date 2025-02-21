
#include "Components/Character/SkillNumber/UltSkillNumber_16.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/SuperiorMode.h"

UUltSkillNumber_16::UUltSkillNumber_16()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_16");
}

void UUltSkillNumber_16::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_16::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
	OnSkill();
}

void UUltSkillNumber_16::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);

	Activate_Skill();
}

void UUltSkillNumber_16::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SuperiorMode = GetWorld()->SpawnActor<ASuperiorMode>(OwnerCharacter->GetAPSkillHubComponent()->GetSuperiorMode(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!SuperiorMode.IsValid()) return;
	SuperiorMode->SetOwner(OwnerCharacter.Get());
	SuperiorMode->SetSkill(SkillAbilityNestingData, this);	
    SuperiorMode->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules ::KeepWorldTransform);

}

void UUltSkillNumber_16::SkillEnd()
{
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_16::UpdateSkillData()
{

}
