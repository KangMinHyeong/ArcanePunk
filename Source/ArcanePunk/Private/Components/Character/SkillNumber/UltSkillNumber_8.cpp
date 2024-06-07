
#include "Components/Character/SkillNumber/UltSkillNumber_8.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Skill/ArcaneMeteor.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_8::UUltSkillNumber_8()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_8");
}

void UUltSkillNumber_8::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_8::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
        if(!CheckSkillCondition()) return;
        OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_SkillRange();
	}
    
}

void UUltSkillNumber_8::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}

	ActivateSkillRange_Target(UltSkill8_TargetRange, UltSkill8_TargetRange_2, ESkillRangeType::Two_Circle);
	if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetMaxDist(UltSkill8_LimitDistance);
	if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(UltSkill8_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void UUltSkillNumber_8::OnSkill()
{
    Super::OnSkill();
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_8_Montage();
    if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
    SpawnLocation = SkillRange_TWoCircle->GetActorLocation();
    CharacterRotation_TwoCircle();
    OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_8::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return; 

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneMeteor = GetWorld()->SpawnActor<AArcaneMeteor>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneMeteor(), SpawnLocation , FRotator::ZeroRotator);
	if(!ArcaneMeteor.IsValid()) return; 
    ArcaneMeteor->SetOwner(OwnerCharacter.Get());
    ArcaneMeteor->SetRadius(UltSkill8_TargetRange, UltSkill8_TargetRange_2);
	ArcaneMeteor->SetSkill(SkillAbilityNestingData, this);	

    SkillEnd();
}

void UUltSkillNumber_8::SkillEnd()
{
	Super::SkillEnd();
	Remove_Skill();
}

void UUltSkillNumber_8::UpdateSkillData()
{

}
