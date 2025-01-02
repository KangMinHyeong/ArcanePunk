
#include "Components/Character/SkillNumber/SkillNumber3.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameElements/SpawnPoint/APSpawnPointBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber3::USkillNumber3()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_3");
}

void USkillNumber3::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber3::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}


void USkillNumber3::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillNumber3::PlaySkill()
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

void USkillNumber3::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_3);

	ActivateSkillRange_Target(Skill3_TargetRange, Skill3_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill3_LimitDist);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(Skill3_LimitDist);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber3::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	if(!SkillRange_Target.IsValid()) return; if(!Skilling) return;
	
	OwnerAnim->PlaySkill_3_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SpawnAttackSphere();
	Remove_Skill();
}

void USkillNumber3::Remove_Skill()
{
	Super::Remove_Skill();
}

void USkillNumber3::SpawnAttackSphere()
{
	if(!OwnerCharacter.IsValid() || !SkillRange_Target.IsValid()) return; 

	bool bStun = false;
	if(SkillRange_Target.IsValid()) bStun = SkillRange_Target->GetbStun();

	// OwnerCharacter->GetAttackComponent()->MultiAttack(SkillRange_Target->GetActorLocation(), false, 1.2f, bStun, 3.0f ,true, Skill3_TargetRange * SkillRange_Target->GetActorScale3D().X);
	OwnerCharacter->GetAttackComponent()->MultiAttack(SkillRange_Target->GetActorLocation(), SkillRange_Target->GetActorLocation(), Skill3_TargetRange * SkillRange_Target->GetActorScale3D().X, 1.2f, 5, bStun, 3.0f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnerCharacter->Skill3_Effect, SkillRange_Target->GetActorLocation(), OwnerCharacter->GetActorRotation());
}

void USkillNumber3::UpdateSkillData()
{
}