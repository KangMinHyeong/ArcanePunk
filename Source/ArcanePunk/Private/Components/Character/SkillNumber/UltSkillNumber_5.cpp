
#include "Components/Character/SkillNumber/UltSkillNumber_5.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ShadowShuriken.h"

UUltSkillNumber_5::UUltSkillNumber_5()
{
	OriginCoolTime = 7.0f;
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
        if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
        bActivate = true;
        OwnerCharacter->SetDoing(true);
        Skilling = true;
        OnSkill();
    }
    
}

void UUltSkillNumber_5::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
    OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	
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
	ShadowShuriken->SetSkill(SkillAbilityNestingData);	

	OwnerCharacter->SetDoing(false);
    Remove_Skill();
}

void UUltSkillNumber_5::SkillEnd()
{
	bActivate = false; 
	OwnerCharacter->SetHideMode(false);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void UUltSkillNumber_5::UpdateSkillData()
{
}