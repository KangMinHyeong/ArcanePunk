
#include "Components/Character/SkillNumber/UltSkillNumber_1.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneRain.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_1::UUltSkillNumber_1()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_1");
}

void UUltSkillNumber_1::BeginPlay()
{
	Super::BeginPlay();

}

void UUltSkillNumber_1::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	if(!CheckSkillCondition()) return;

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
	ArcaneRain->SetSkill(SkillAbilityNestingData, this);	
	// ArcaneRain->SetRainEffect();

	SkillEnd();
}

void UUltSkillNumber_1::SkillEnd()
{
	Super::SkillEnd();
	// if(ArcaneRain.IsValid()) ArcaneRain->DestroySKill();
}

void UUltSkillNumber_1::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	float Cool = SkillNameListData.CoolTime;
	for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
		if(It.Key == 3) {
			UpdatAbilityData(EEnHanceType::Gold, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_SubtractMultiple(Cool, AbilityData->Coefficient_X, It.Value); // 쿨타임 강화
		} 
	}
	CurrentCoolTime = Cool;
}
