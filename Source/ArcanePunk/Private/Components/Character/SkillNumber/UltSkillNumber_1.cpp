
#include "Components/Character/SkillNumber/UltSkillNumber_1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneRain.h"

void UUltSkillNumber_1::BeginPlay()
{
	Super::BeginPlay();

}

void UUltSkillNumber_1::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant); Ult는 강화 X(후에는 바뀔수도)
}

void UUltSkillNumber_1::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->SetDoing(true);
    SetAbility(WhichKey);

	SkillKey = WhichKey;
	CurrentSkillType = SkillType;

	OnSkill();
}

void UUltSkillNumber_1::OnSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_1_Montage();
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_1::Remove_Skill()
{
}

void UUltSkillNumber_1::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ArcaneRain = GetWorld()->SpawnActor<AArcaneRain>(OwnerCharacter->GetArcaneRainClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!ArcaneRain.IsValid()) return;
	// ArcaneRain->SetSkill(CurrentSkillType, CurrentSkillAbility);	
	ArcaneRain->SetOwner(OwnerCharacter);
	ArcaneRain->SetRainEffect();
}

void UUltSkillNumber_1::SkillEnd()
{
	if(ArcaneRain.IsValid()) ArcaneRain->DestroySKill();
}
