
#include "Components/Character/SkillNumber/UltSkillNumber_4.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Skill/BlackHole.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_4::UUltSkillNumber_4()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_4");
    UltSkill4_TargetRange = 400.0f;
	UltSkill4_TargetRange_2 = 250.0f;
    UltSkill4_LimitDistance = 650.0f;
}

void UUltSkillNumber_4::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_4::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
        if(bActivate)
        {
            
        }
        else
        {
			if(!CheckSkillCondition()) return;
            OwnerCharacter->SetDoing(true);
			Skilling = true;
			Spawn_SkillRange();
        }
		
		// OnSkill();
	}
    
}

void UUltSkillNumber_4::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}

	ActivateSkillRange_Target(UltSkill4_TargetRange, UltSkill4_TargetRange_2, ESkillRangeType::Two_Circle);
	if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetMaxDist(UltSkill4_LimitDistance);
	if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(UltSkill4_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);

}

void UUltSkillNumber_4::OnSkill()
{
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
    OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
	    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_4_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);

	SpawnLocation = SkillRange_TWoCircle->GetActorLocation();

    CharacterRotation_TwoCircle();
}

void UUltSkillNumber_4::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BlackHole = GetWorld()->SpawnActor<ABlackHole>(OwnerCharacter->GetAPSkillHubComponent()->GetBlackHoleClass(), SpawnLocation , FRotator::ZeroRotator);
	if(!BlackHole.IsValid()) return; 
    BlackHole->SetOwner(OwnerCharacter.Get());
    BlackHole->SetHoleRadius(UltSkill4_TargetRange, UltSkill4_TargetRange_2);
	BlackHole->SetSkill(SkillAbilityNestingData, this);	

	bActivate = true;
	Remove_Skill();
}

void UUltSkillNumber_4::SkillEnd()
{
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_4::UpdateSkillData()
{

}
