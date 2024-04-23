
#include "Components/Character/SkillNumber/SkillNumber6.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/Shouting.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber6::USkillNumber6()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_6");
}

void USkillNumber6::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber6::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	
}

void USkillNumber6::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}

	OwnerCharacter->SetDoing(true);
	
	OnSkill();

	// if(CurrentSkillAbility.Contains(ESkillAbility::Homing))
	// {
	// 	if(CheckSmartKey(WhichKey, OwnerCharacter))
	// 	{
	// 		OnSkill(SkillType);
	// 	}
	// 	else
	// 	{
	// 		auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	// 		SetMouseCursor(PC, ESkillCursor::Crosshairs);
	// 		PC->DisplayHomingUI(ESkillNumber::Skill_4, SkillType);
	// 	}
	// }
	// else
	// {
		
	// }
}

void USkillNumber6::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_6_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber6::Remove_Skill()
{
}

void USkillNumber6::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Shouting = GetWorld()->SpawnActor<AShouting>(OwnerCharacter->GetAPSkillHubComponent()->GetShoutingClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!Shouting.IsValid()) return;
	Shouting->SetSkill(SkillAbilityNestingData);	
	Shouting->SetOwner(OwnerCharacter.Get());
	Shouting->SetShoutingEffect();
}

void USkillNumber6::SkillEnd()
{
	if(Shouting.IsValid()) Shouting->DestroySKill();
}

void USkillNumber6::UpdateSkillData()
{
}