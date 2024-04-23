
#include "Components/Character/SkillNumber/UltSkillNumber_1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneRain.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_1::UUltSkillNumber_1()
{
	OriginCoolTime = 15.0f;
}

void UUltSkillNumber_1::BeginPlay()
{
	Super::BeginPlay();

}

void UUltSkillNumber_1::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}

	OwnerCharacter->SetDoing(true);

	OnSkill();
}

void UUltSkillNumber_1::OnSkill()
{
	Super::OnSkill();
	if(!OwnerCharacter.IsValid()) return;
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_1_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_1::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ArcaneRain = GetWorld()->SpawnActor<AArcaneRain>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneRainClass(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	if(!ArcaneRain.IsValid()) return;
	// ArcaneRain->SetSkill(CurrentSkillType, CurrentSkillAbility);	
	ArcaneRain->SetOwner(OwnerCharacter.Get());
	ArcaneRain->SetRainEffect();
}

void UUltSkillNumber_1::SkillEnd()
{
	if(ArcaneRain.IsValid()) ArcaneRain->DestroySKill();
}

void UUltSkillNumber_1::UpdateSkillData()
{
}
