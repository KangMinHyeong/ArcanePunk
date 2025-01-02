
#include "Components/Character/SkillNumber/UltSkillNumber_14.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/WindCutter.h"

UUltSkillNumber_14::UUltSkillNumber_14()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_14");
    UltSkill14_LimitDistance = 1500.0f;
	UltSkill14_Angle = 0.10f;
}

void UUltSkillNumber_14::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_14::PlaySkill()
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

void UUltSkillNumber_14::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}

	ActivateSkillRange_Target(UltSkill14_LimitDistance, UltSkill14_LimitDistance, ESkillRangeType::SectorCircle);
	if(SkillRange_Target.IsValid())
    {
        SkillRange_Target->SetMaxDist(0.01f);
	    SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	
        SkillRange_Target->SetAngle(UltSkill14_Angle);
    } 

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void UUltSkillNumber_14::OnSkill()
{
    Super::OnSkill();
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_14_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);

    SetWindInform();
    CharacterRotation_Sector();
}

void UUltSkillNumber_14::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WindCutter = GetWorld()->SpawnActor<AWindCutter>(OwnerCharacter->GetAPSkillHubComponent()->GetWindCutter(), OwnerCharacter->GetActorLocation() , SpawnRotation);
	if(!WindCutter.IsValid()) return; 
    WindCutter->SetOwner(OwnerCharacter.Get());
    WindCutter->SetMaxWindWitdh(UltSkill14_Width);
    WindCutter->SetMaxLocation(TargetLocation);
    WindCutter->SetMaxRadius(UltSkill14_LimitDistance);
	WindCutter->SetSkill(SkillAbilityNestingData, this);	
    
	SkillEnd();
}

void UUltSkillNumber_14::SetWindInform()
{
    float Degree = UltSkill14_Angle * 180.0f; Degree = FMath::DegreesToRadians(Degree);
    UltSkill14_Width = UltSkill14_LimitDistance * FMath::Sin(Degree);

    TargetLocation = SkillRange_Target->GetActorForwardVector() * UltSkill14_LimitDistance * FMath::Cos(Degree);
    SpawnRotation = SkillRange_Target->GetActorRotation();

	// float Degree = UltSkill14_Angle * 180.0f; Degree = FMath::DegreesToRadians(Degree);
    // UltSkill14_Width = UltSkill14_LimitDistance * FMath::Sin(Degree);

	// FVector Unit = OwnerCharacter->GetMesh()->GetComponentLocation() - SkillRange_Target->GetDecalComponent()->GetComponentLocation();
    // Unit = Unit/Unit.Size(); 
    
    // TargetLocation = Unit *UltSkill14_LimitDistance * FMath::Cos(Degree); // + OwnerCharacter->GetMesh()->GetComponentLocation();
	// SpawnRoatation = SkillRange_Target->GetActorRotation();
	
    // TargetLocation = SpawnLocation - Unit * UltSkill9_Length;   
}

void UUltSkillNumber_14::SkillEnd()
{
	Super::SkillEnd();
	Remove_Skill();
}

void UUltSkillNumber_14::UpdateSkillData()
{

}
