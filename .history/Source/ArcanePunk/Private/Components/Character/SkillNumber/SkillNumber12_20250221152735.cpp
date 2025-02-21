
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

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void USkillNumber12::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_12_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber12::Activate_Skill()
{
	Super::Activate_Skill();
    if(!OwnerCharacter.IsValid()) return;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneRage = GetWorld()->SpawnActor<AArcaneRage>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneRageClass(), OwnerCharacter->GetActorLocation() + FVector(0,0,SpawnAddLocation), OwnerCharacter->GetActorRotation(), SpawnParams);
	
    if(!ArcaneRage) return;
	ArcaneRage->SetOwner(OwnerCharacter.Get());
	ArcaneRage->SetSkill(SkillAbilityNestingData, this);	
    ArcaneRage->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules ::KeepWorldTransform);
	
	Remove_Skill();
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
}

void USkillNumber12::SkillEnd()
{
	Super::SkillEnd();
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void USkillNumber12::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;

	float Cool = SkillNameListData.CoolTime;
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
	Cool = FMath::Max(SkillNameListData.CoolTime * 0.4f, Cool);
	CurrentCoolTime = Cool;
}