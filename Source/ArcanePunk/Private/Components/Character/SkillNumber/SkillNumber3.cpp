
#include "Components/Character/SkillNumber/SkillNumber3.h"

#include "Character/ArcanePunkCharacter.h"
#include "SpawnPoint/APSpawnPointBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"

void USkillNumber3::BeginPlay()
{
	Super::BeginPlay();

}

void USkillNumber3::AddAbilityList()
{
	EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	EnableSkillAbilityList.Add(ESkillAbility::Stun);
}


void USkillNumber3::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillNumber3::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;	

	SkillKey = WhichKey;

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);

		SetAbility(WhichKey);
		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill3();
	}
	
}

void USkillNumber3::Spawn_Skill3()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;
	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	if(!CheckSmartKey(SkillKey, OwnerCharacter)) OwnerCharacterPC->PreventOtherClick(ESkillNumber::Skill_3);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ActivateSkillRange_Target(Skill3_TargetRange, Skill3_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(OwnerCharacter->GetSkill3_LimitDist());
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(CurrentSkillType, CurrentSkillAbility);

	ActivateSkillRange_Round(OwnerCharacter->GetSkill3_LimitDist());
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(CurrentSkillType, CurrentSkillAbility);

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber3::OnSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC) return;
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
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter || !SkillRange_Target.IsValid()) return; 

	bool bStun = false;
	if(SkillRange_Target.IsValid()) bStun = SkillRange_Target->GetbStun();

	OwnerCharacter->GetAttackComponent()->MultiAttack(SkillRange_Target->GetActorLocation(), false, 1.2f, bStun, 3.0f ,true, Skill3_TargetRange * SkillRange_Target->GetActorScale3D().X);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnerCharacter->Skill3_Effect, SkillRange_Target->GetActorLocation(), OwnerCharacter->GetActorRotation());
	
}