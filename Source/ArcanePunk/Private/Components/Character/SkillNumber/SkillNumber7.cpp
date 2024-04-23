
#include "Components/Character/SkillNumber/SkillNumber7.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneCutter.h"

USkillNumber7::USkillNumber7()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_7");
}

void USkillNumber7::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber7::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber7::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
		OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_Skill7();
	}
}

void USkillNumber7::Spawn_Skill7()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_7);}
	
	ActivateSkillRange_Target(Skill7_Wide, Skill7_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill7_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey))
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
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
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
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneCutter = GetWorld()->SpawnActor<AArcaneCutter>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneCutterClass(), OwnerCharacter->GetMesh()->GetComponentLocation()+OwnerCharacter->GetActorForwardVector()*SpawnAddLocation , OwnerCharacter->GetActorRotation());
	if(!ArcaneCutter.IsValid()) return; 
	ArcaneCutter->SetSkill(SkillAbilityNestingData);	
    float Size = ArcaneCutter->GetActorScale3D().Y/ ArcaneCutter->DefaultSize;
    ArcaneCutter->SetActorLocation( ArcaneCutter->GetActorLocation() + (OwnerCharacter->GetActorUpVector()*150.0f*Size));
	ArcaneCutter->SetOwner(OwnerCharacter.Get());
    ArcaneCutter->SetDeadTime((Skill7_LimitDistance * 2.0f - SpawnAddLocation - ArcaneCutter->GetTriggerWide() * 2.0f) / ArcaneCutter->GetCutterSpeed()); // 거리 = 속력 * 시간 으로 DeadTime 수정
	// if(SkillRange_Target) ArcaneBeam->SetDistance(SkillRange_Target->GetTargetDistance() * 2.0f);
	// if(SkillRange_Target) ArcaneBeam->SetWide(SkillRange_Target->GetTargetWide());
	Remove_Skill();
}

void USkillNumber7::SkillEnd()
{
	// if(ArcaneBeam.IsValid()) ArcaneBeam->DestroySKill();
}

void USkillNumber7::UpdateSkillData()
{
}