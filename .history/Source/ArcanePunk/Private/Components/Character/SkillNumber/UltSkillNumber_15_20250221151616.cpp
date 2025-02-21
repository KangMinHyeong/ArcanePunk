
#include "Components/Character/SkillNumber/UltSkillNumber_15.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/BomberMan.h"

UUltSkillNumber_15::UUltSkillNumber_15()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_15");
}

void UUltSkillNumber_15::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_15::PlaySkill()
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

void UUltSkillNumber_15::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_15_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_15::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BomberMan = GetWorld()->SpawnActor<ABomberMan>(OwnerCharacter->GetAPSkillHubComponent()->GetBomberMan(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!BomberMan.IsValid()) return;
	BomberMan->SetOwner(OwnerCharacter.Get());
	BomberMan->SetSkill(SkillAbilityNestingData, this);	
    BomberMan->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules ::KeepWorldTransform);

	Skilling = false;
}

void UUltSkillNumber_15::SkillEnd()
{
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_15::UpdateSkillData()
{

}