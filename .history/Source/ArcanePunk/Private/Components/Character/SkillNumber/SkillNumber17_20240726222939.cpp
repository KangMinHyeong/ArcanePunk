
#include "Components/Character/SkillNumber/SkillNumber17.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APSkillHubComponent.h"

USkillNumber17::USkillNumber17()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_17");
}

void USkillNumber17::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber17::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
	Skilling = true;
	OnSkill();
}

void USkillNumber17::OnSkill()
{
	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	
	Activate_Skill();
    Remove_Skill();
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
}

void USkillNumber17::Activate_Skill()
{
	Super::Activate_Skill();
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAttached(OwnerCharacter->GetAPSkillHubComponent()->GetSkill17Effect(), OwnerCharacter->GetMesh(), TEXT("Skill17Effect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
    NC->SetVariableFloat(TEXT("Time"), BuffTime);

    OwnerCharacter->GetBuffComp()->ATKUp(BuffCoefficient, BuffTime); 
    GetWorld()->GetTimerManager().SetTimer(Skill17_TimerHandle, this, &USkillNumber17::SkillEnd, BuffTime, false);
}

void USkillNumber17::SkillEnd()
{
	Super::SkillEnd();
    GetWorld()->GetTimerManager().ClearTimer(Skill17_TimerHandle);
    // NC->DestroyComponent();
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void USkillNumber17::UpdateSkillData()
{
}
