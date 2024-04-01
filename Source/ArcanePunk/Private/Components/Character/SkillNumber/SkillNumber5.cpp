
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

void USkillNumber5::BeginPlay()
{
	Super::BeginPlay();
	SkillAbilityNestingData.SkillName = TEXT("Skill_5");
	InitIncreasingSpeed = IncreasingSpeed;
}

void USkillNumber5::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	DamageCoefficient = FMath::FInterpConstantTo(DamageCoefficient, MaxDamageCoefficient, DeltaTime, IncreasingSpeed);

}

void USkillNumber5::AddAbilityList()
{
	// EnableSkillAbilityList.Add(ESkillAbility::Gigant);
	// // EnableSkillAbilityList.Add(ESkillAbility::Homing);
	// EnableSkillAbilityList.Add(ESkillAbility::Stun);
}

void USkillNumber5::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	switch(SkillType)
	{
		case ESkillTypeState::Type_None:
		SkillKey = WhichKey;
		break;
		case ESkillTypeState::Type_Q:
		SkillKey = ESkillKey::Q;
		break;
		case ESkillTypeState::Type_E:
		SkillKey = ESkillKey::E;
		break;
		case ESkillTypeState::Type_R:
		SkillKey = ESkillKey::R;
		break;
	}

	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		OwnerCharacter->SetDoing(true);
		SetAbility(WhichKey);

		Skilling = true;
		CurrentSkillType = SkillType;
		Spawn_Skill5();

		OnSkill5();
	}
}

void USkillNumber5::Spawn_Skill5()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ActivateSkillRange_Target(Skill5_Wide, Skill5_LimitDistance, ESkillRangeType::Arrow);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Skill5_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey, OwnerCharacter))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	SpawnChargeEffect();

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber5::OnSkill5()
{
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlaySkill_5_Montage();
	OnSkill();
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, true);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void USkillNumber5::OnSkill()
{
	Super::OnSkill();
}

void USkillNumber5::Remove_Skill()
{
	Super::Remove_Skill();
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
}

void USkillNumber5::Activate_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	RemoveEffect();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArcaneBeam = GetWorld()->SpawnActor<AArcaneBeam>(OwnerCharacter->GetArcaneBeamClass(), OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);
	if(!ArcaneBeam.IsValid()) return; 
	ArcaneBeam->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BeamPoint"));
	ArcaneBeam->SetSkill(SkillAbilityNestingData);	
	ArcaneBeam->SetOwner(OwnerCharacter);
	if(SkillRange_Target.IsValid()) ArcaneBeam->SetDistance(SkillRange_Target->GetTargetDistance() * 2.0f);
	if(SkillRange_Target.IsValid()) ArcaneBeam->SetWide(SkillRange_Target->GetTargetWide());
	SetComponentTickEnabled(false);
	ArcaneBeam->SetBeamEffect(DamageCoefficient); DamageCoefficient = 1.0f;  IncreasingSpeed = InitIncreasingSpeed;
	Remove_Skill();
}

void USkillNumber5::SkillEnd()
{
	if(ArcaneBeam.IsValid()) ArcaneBeam->DestroySKill();
}

void USkillNumber5::SpawnChargeEffect()
{
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	ChargeEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OwnerCharacter->GetChargeEffect(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
	// ChargeEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(OwnerCharacter->GetChargeEffect(), OwnerCharacter->GetMesh(), TEXT("BeamPoint"), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
}

void USkillNumber5::RemoveEffect()
{
	if(ChargeEffectComp.IsValid()) ChargeEffectComp->DeactivateImmediate();
	if(ChargeEnhanceEffectComp.IsValid()) ChargeEnhanceEffectComp->DeactivateImmediate();
}

void USkillNumber5::Enhance()
{
	IncreasingSpeed = InitIncreasingSpeed * 3.0;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	ChargeEnhanceEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(OwnerCharacter->GetChargeEnhanceEffect(), OwnerCharacter->GetMesh(), TEXT("BeamPoint"), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
}