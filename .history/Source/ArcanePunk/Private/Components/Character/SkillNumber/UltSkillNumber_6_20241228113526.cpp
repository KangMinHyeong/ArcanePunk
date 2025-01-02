
#include "Components/Character/SkillNumber/UltSkillNumber_6.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/WindRush.h"

UUltSkillNumber_6::UUltSkillNumber_6()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_6");
}

void UUltSkillNumber_6::BeginPlay()
{
	Super::BeginPlay();

}

void UUltSkillNumber_6::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
        Skilling = true;
        Spawn_SkillRange();
	}
}

void UUltSkillNumber_6::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    // if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}
	
	ActivateSkillRange_Target(UltSkill6_Width, UltSkill6_LimitDistance * 0.5f, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(UltSkill6_LimitDistance * 0.5f);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void UUltSkillNumber_6::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_6_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	SpawnLocation = SkillRange_Target->GetDecalComponent()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation();
    CharacterRotation();
}

void UUltSkillNumber_6::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WindRush = GetWorld()->SpawnActor<AWindRush>(OwnerCharacter->GetAPSkillHubComponent()->GetWindRush(), OwnerCharacter->GetActorLocation() , OwnerCharacter->GetActorRotation());
	if(!WindRush.IsValid()) return; 
    WindRush->SetOwner(OwnerCharacter.Get());
    WindRush->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    WindRush->SetWindRushWidth(UltSkill6_Width);
    WindRush->SetTargetLocation(SpawnLocation, UltSkill6_LimitDistance);
	WindRush->SetSkill(SkillAbilityNestingData, this);	

    // bActivate = true; 

    
}

void UUltSkillNumber_6::SkillEnd()
{
	Super::SkillEnd();
    OwnerCharacter->SetDoing(false);
    Remove_Skill();
}

void UUltSkillNumber_6::UpdateSkillData()
{

}
