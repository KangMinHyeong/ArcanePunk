
#include "Components/Character/SkillNumber/SkillNumber16.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber16::USkillNumber16()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_16");
}

void USkillNumber16::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber16::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
	Skilling = true;
	OnSkill();
}

void USkillNumber16::OnSkill()
{
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	
	Activate_Skill();
    Remove_Skill();
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
}

void USkillNumber16::Activate_Skill()
{
	Super::Activate_Skill();
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAttached(OwnerCharacter->GetAPSkillHubComponent()->GetSkill16Effect(), OwnerCharacter->GetMesh(), TEXT("Skill16Effect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
    NC->SetNiagaraVariableFloat(TEXT("Time"), BuffTime);

    OwnerCharacter->GetCrowdControlComp()->FastState(BuffCoefficient, BuffTime);
    OwnerCharacter->GetBuffComp()->ATKSpeedUp(BuffCoefficient, BuffTime); 
    GetWorld()->GetTimerManager().SetTimer(Skill16_TimerHandle, this, &USkillNumber16::SkillEnd, BuffTime, false);
}

void USkillNumber16::SkillEnd()
{
	Super::SkillEnd();
    GetWorld()->GetTimerManager().ClearTimer(Skill16_TimerHandle);
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
}

void USkillNumber16::UpdateSkillData()
{
}