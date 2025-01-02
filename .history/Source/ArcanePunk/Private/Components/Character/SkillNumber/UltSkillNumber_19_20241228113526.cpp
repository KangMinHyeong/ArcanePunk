
#include "Components/Character/SkillNumber/UltSkillNumber_19.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Skill/ArcaneAlterEgo.h"

UUltSkillNumber_19::UUltSkillNumber_19()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_19");

	UltSkill19_LimitDistance = 215.0f;
}

void UUltSkillNumber_19::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_19::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_SkillRange();
	}
}

void UUltSkillNumber_19::Spawn_SkillRange()
{
	Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}

	ActivateSkillRange_Target(UltSkill19_TargetRange, UltSkill19_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(UltSkill19_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(UltSkill19_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void UUltSkillNumber_19::OnSkill()
{
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);

	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);

    SpawnLocation = SkillRange_Target->GetActorLocation(); // SpawnLocation.Z = OwnerCharacter->GetActorLocation().Z;
	
	SkillSetting(true);

    GetWorld()->GetTimerManager().SetTimer(AlterEgoTimerHandle, this, &UUltSkillNumber_19::Activate_Skill, AlterEgoDelay, false);
}

void UUltSkillNumber_19::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;

	SkillSetting(false);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto AlterEgo = GetWorld()->SpawnActor<AArcaneAlterEgo>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneAlterEgo(), SpawnLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!AlterEgo) return;
	AlterEgo->SetOwner(OwnerCharacter.Get());
	AlterEgo->SetSkill(SkillAbilityNestingData, this);	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OwnerCharacter->GetAPSkillHubComponent()->GetSkill20Effect(), AlterEgo->GetActorLocation(), AlterEgo->GetActorRotation(), AlterEgoEffectScale);
	
	bActivate = true;

    OwnerCharacter->SetDoing(false);
    OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Remove_Skill();
}

void UUltSkillNumber_19::SkillSetting(bool Start)
{
	if(Start)
	{
		OwnerCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		OwnerCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		OwnerCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		OwnerCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	OwnerCharacter->SetHideModeOnlyBool(Start);
	OwnerCharacter->SetActorHiddenInGame(Start);
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OwnerCharacter->GetAPSkillHubComponent()->GetSkill20Effect(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), AlterEgoEffectScale);
}

void UUltSkillNumber_19::SkillEnd()
{	
	OwnerCharacter->SetDoing(false);
	bActivate = false;
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_19::UpdateSkillData()
{

}

