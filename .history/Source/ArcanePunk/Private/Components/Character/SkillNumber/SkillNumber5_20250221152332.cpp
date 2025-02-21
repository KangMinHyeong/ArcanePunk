
#include "Components/Character/SkillNumber/SkillNumber5.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/ArcaneBeam.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

USkillNumber5::USkillNumber5()
{
	SkillAbilityNestingData.SkillName = TEXT("Skill_5");

}

void USkillNumber5::BeginPlay()
{
	Super::BeginPlay();
}

void USkillNumber5::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if(SkillRange_Target.IsValid())	GetOwner()->SetActorRotation(FRotationMatrix::MakeFromX( SkillRange_Target->GetRotVector()).Rotator());
	
}

void USkillNumber5::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_SkillRange();

		OnSkill();
	}
}

void USkillNumber5::Spawn_SkillRange()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

	ActivateSkillRange_Target(Skill5_Wide, Skill5_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill5_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	// if(CheckSmartKey(SkillKey))
	// {
	// 	OwnerCharacterPC->bShowMouseCursor = true;
	// 	CursorImmediately();
	// 	SkillRange_Target->SetActorHiddenInGame(true);
	// }

	OwnerCharacter->SetDoing(false);
	// SetComponentTickEnabled(true);
}

void USkillNumber5::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->SetDoing(true);

	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumber5::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumber5::SkillEnd);
	OwnerCharacter->OnSkillChargingTrigger.AddDynamic(this, &USkillNumber5::Enhance);
	OwnerCharacter->OnSkillEnhanceTrigger.AddDynamic(this, &USkillNumber5::DoubleEnhance);
	OwnerCharacter->OnSkillRotationTrigger.AddDynamic(this, &USkillNumber5::Remove_Skill);
	    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_5_Montage();
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	Activate_Skill_5();
}

void USkillNumber5::Activate_Skill_5()
{
	if(!OwnerCharacter.IsValid()) return;
	// RemoveEffect();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneBeam = GetWorld()->SpawnActor<AArcaneBeam>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneBeamClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!ArcaneBeam.IsValid()) return; 
	ArcaneBeam->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BeamPoint"));
	ArcaneBeam->SetOwner(OwnerCharacter.Get());
	ArcaneBeam->SetDistance(Skill5_LimitDistance * 2.0f - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
	ArcaneBeam->SetWide(Skill5_Wide);
	ArcaneBeam->SetSkill(SkillAbilityNestingData, this);	

}

void USkillNumber5::Activate_Skill()
{
	Super::Activate_Skill();
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;
	
	ArcaneBeam->SetBeamEffect();	
}

void USkillNumber5::Remove_Skill()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	OwnerCharacter->OnSkillRotationTrigger.RemoveDynamic(this, &USkillNumber5::Remove_Skill);
	CharacterRotation();
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->StopSkill_5_Montage();

	SetComponentTickEnabled(false);
	if(SkillRange_Target.IsValid()) SkillRange_Target->Destroy();
	OwnerCharacterPC->SetMouseCursor();
	USkillDataManager::CursorImmediately();
	Skilling = false;

	if(!ArcaneBeam.IsValid()) return;
	ArcaneBeam->SetbCharging(false);
}

void USkillNumber5::SkillEnd()
{
	Super::SkillEnd();
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;

	// OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &USkillNumber5::Activate_Skill);
	OwnerCharacter->OnSkillChargingTrigger.RemoveDynamic(this, &USkillNumber5::Enhance);
	OwnerCharacter->OnSkillEnhanceTrigger.RemoveDynamic(this, &USkillNumber5::DoubleEnhance);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumber5::OnSkill);

	if(ArcaneBeam.IsValid()) ArcaneBeam->DestroySKill();
}

void USkillNumber5::Enhance()
{
	Super::Enhance();
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;
	ArcaneBeam->OnCharging();
}

void USkillNumber5::DoubleEnhance()
{
	Super::DoubleEnhance();
	if(!OwnerCharacter.IsValid() || !ArcaneBeam.IsValid()) return;
	ArcaneBeam->OnChargingEnhance();
}

void USkillNumber5::UpdateSkillData()
{
	Super::UpdateSkillData();
	if(!OwnerCharacter.IsValid()) return;
	
	float Dist = Skill5_LimitDistance_Origin;
	float Wide = Skill5_Wide_Origin;
	float Cool = SkillNameListData.CoolTime;
	for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
		if(It.Key == 2) {
			UpdatAbilityData(EEnHanceType::Silver, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Dist, AbilityData->Coefficient_X, It.Value); // 사거리 강화}
		} 
		if(It.Key == 3) {
			UpdatAbilityData(EEnHanceType::Silver, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_AddMultiple(Wide, AbilityData->Coefficient_X, It.Value); // 사거리 강화}
		} 
	}
	for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
		if(It.Key == 3) {
			UpdatAbilityData(EEnHanceType::Gold, It.Key); 
			OwnerCharacter->GetAPSkillAbility()->Coefficient_SubtractMultiple(Cool, AbilityData->Coefficient_X, It.Value); // 쿨타임 강화
		} 
	}
    // for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    // {
    // }
    // for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    // {
    // }
	Skill5_LimitDistance = Dist;
	Skill5_Wide = Wide;
	CurrentCoolTime = Cool;
}
