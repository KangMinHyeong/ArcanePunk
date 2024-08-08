
#include "Components/Character/SkillNumber/SkillNumber19.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/MoonSlash.h"

USkillNumber19::USkillNumber19()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_19");

    Skill19_Angle = 0.45f;
}

void USkillNumber19::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber19::PlaySkill()
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

void USkillNumber19::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_19);}

	ActivateSkillRange_Target(Skill19_LimitDistance, Skill19_LimitDistance, ESkillRangeType::SectorCircle);
	if(SkillRange_Target.IsValid())
    {
        SkillRange_Target->SetMaxDist(0.01f);
	    SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	
        SkillRange_Target->SetAngle(Skill19_Angle);
    } 

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber19::OnSkill()
{
    Super::OnSkill();
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
	    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_19_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);

    SetSlashInform();
    CharacterRotation_Sector();
}

void USkillNumber19::Activate_Skill()
{
	Super::Activate_Skill();
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MoonSlash = GetWorld()->SpawnActor<AMoonSlash>(OwnerCharacter->GetAPSkillHubComponent()->GetMoonSlash(), OwnerCharacter->GetActorLocation() , SpawnRotation);
	if(!MoonSlash.IsValid()) return; 
    MoonSlash->SetOwner(OwnerCharacter.Get());
    MoonSlash->SetMoonSlashWidth(Skill19_LimitDistance * 0.5f);
    MoonSlash->SetMoonSlashAngle(Skill19_Angle); // 각 세팅
    MoonSlash->SetSkill(SkillAbilityNestingData, this);	
    // WindCutter->SetMaxWindWitdh(UltSkill14_Width);
    // WindCutter->SetMaxLocation(TargetLocation);
    // WindCutter->SetMaxRadius(UltSkill14_LimitDistance);
	
    
	Remove_Skill();
}

void USkillNumber19::SetSlashInform()
{
    float Degree = Skill19_Angle * 180.0f; Degree = FMath::DegreesToRadians(Degree);

    // TargetLocation = SkillRange_Target->GetActorForwardVector() * Skill19_LimitDistance * FMath::Cos(Degree);
    SpawnRotation = SkillRange_Target->GetActorRotation();
}

void USkillNumber19::SkillEnd()
{
	Super::SkillEnd();
}

void USkillNumber19::UpdateSkillData()
{

}

