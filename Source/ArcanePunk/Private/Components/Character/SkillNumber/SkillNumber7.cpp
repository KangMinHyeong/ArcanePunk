
#include "Components/Character/SkillNumber/SkillNumber7.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneCutter.h"

void USkillNumber7::BeginPlay()
{
	Super::BeginPlay();
	SkillAbilityNestingData.SkillName = TEXT("Skill_7");
}

void USkillNumber7::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber7::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);
		SetAbility(WhichKey);
        SkillKey = WhichKey;
		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill7();
	}
}

void USkillNumber7::Spawn_Skill7()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    if(!CheckSmartKey(SkillKey, OwnerCharacter)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_7);}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	ActivateSkillRange_Target(Skill7_Wide, Skill7_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill7_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey, OwnerCharacter))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber7::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_7_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
    CharacterRotation();
}

void USkillNumber7::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber7::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneCutter = GetWorld()->SpawnActor<AArcaneCutter>(OwnerCharacter->GetArcaneCutterClass(), OwnerCharacter->GetMesh()->GetComponentLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation());
	if(!ArcaneCutter.IsValid()) return; 
	ArcaneCutter->SetSkill(SkillAbilityNestingData);	
    float Size = ArcaneCutter->GetActorScale3D().Y/ ArcaneCutter->DefaultSize;
    ArcaneCutter->SetActorLocation( ArcaneCutter->GetActorLocation() + (OwnerCharacter->GetActorUpVector()*150.0f*Size));
	ArcaneCutter->SetOwner(OwnerCharacter);
    ArcaneCutter->SetDeadTime((Skill7_LimitDistance * 2.0f - SpawnAddLocation - ArcaneCutter->GetTriggerWide() * 2.0f) / ArcaneCutter->GetCutterSpeed()); // 거리 = 속력 * 시간 으로 DeadTime 수정
	// if(SkillRange_Target) ArcaneBeam->SetDistance(SkillRange_Target->GetTargetDistance() * 2.0f);
	// if(SkillRange_Target) ArcaneBeam->SetWide(SkillRange_Target->GetTargetWide());
	Remove_Skill();
}

void USkillNumber7::SkillEnd()
{
	// if(ArcaneBeam.IsValid()) ArcaneBeam->DestroySKill();
}