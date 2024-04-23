
#include "Components/Character/SkillNumber/SkillNumber12.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneRage.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber12::USkillNumber12()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_12");
}

void USkillNumber12::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber12::AddAbilityList()
{
}

void USkillNumber12::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void USkillNumber12::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_12_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber12::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneRage = GetWorld()->SpawnActor<AArcaneRage>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneRageClass(), OwnerCharacter->GetActorLocation() + FVector(0,0,SpawnAddLocation), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneRage) return;
	ArcaneRage->SetOwner(OwnerCharacter.Get());
	ArcaneRage->SetSkill(SkillAbilityNestingData);	
    ArcaneRage->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules ::KeepWorldTransform);
	Skilling = false;
}

void USkillNumber12::SkillEnd()
{
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber12::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;

	float Cool = OriginCoolTime;
	for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        if(It.Key == 2)
		{
			UpdatAbilityData(EEnHanceType::Silver, It.Key);
			Cool = OwnerCharacter->GetAPSkillAbility()->Coefficient_Multiple_Return(Cool, AbilityData->Coefficient_X, It.Value); // 쿨타임 강화
		} 
	}
    // for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    // {
    // }
    // for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    // {
    // }
	Cool = FMath::Max(OriginCoolTime * 0.4f, Cool);
	CurrentCoolTime = Cool;
}