
#include "Components/Character/SkillNumber/UltSkillNumber_5.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ShadowShuriken.h"

UUltSkillNumber_5::UUltSkillNumber_5()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_5");
}

void UUltSkillNumber_5::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_5::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate)
    {
        if(ShadowShuriken.IsValid()) { ShadowShuriken->CollectShuriken();}
    }
    else
    {
        if(!CheckSkillCondition()) return;
        bActivate = true;
        OwnerCharacter->SetDoing(true);
        Skilling = true;
        OnSkill();
    }
    
}

void UUltSkillNumber_5::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;
    
	OwnerAnim->PlayUltSkill_5_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_5::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetHideMode(true);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ShadowShuriken = GetWorld()->SpawnActor<AShadowShuriken>(OwnerCharacter->GetAPSkillHubComponent()->GetShadowShuriken(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!ShadowShuriken.IsValid()) return;
	ShadowShuriken->SetOwner(OwnerCharacter.Get());
    ShadowShuriken->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform );
	ShadowShuriken->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
    Remove_Skill();
}

void UUltSkillNumber_5::SkillEnd()
{
	bActivate = false; 
	OwnerCharacter->SetHideMode(false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_5::UpdateSkillData()
{
}