
#include "Components/Character/SkillNumber/UltSkillNumber_7.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/CapsuleComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APSpringArmComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_7::UUltSkillNumber_7()
{
    SkillAbilityNestingData.SkillName = TEXT("UltSkill_7");

}

void UUltSkillNumber_7::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_7::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
        OwnerCharacter->SetDoing(true);
		Skilling = true;
		Spawn_SkillRange();

	}
    
}

void UUltSkillNumber_7::Spawn_SkillRange()
{
    Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

	// if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}

	ActivateSkillRange_Target(UltSkill7_TargetRange, UltSkill7_TargetRange, ESkillRangeType::Control_Circle);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(UltSkill7_LimitDistance);
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

	ActivateSkillRange_Round(UltSkill7_LimitDistance);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);

}

void UUltSkillNumber_7::OnSkill()
{
	SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	// OwnerAnim->PlayUltSkill_7_Montage();
    PlayerCollisionChange(false);

    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);

    CharacterRotation();
    TeleportLocation = SkillRange_Target->GetActorLocation(); 
    OwnerCharacter->GetAPSpringArm()->bEnableCameraLag = false;

    GetWorld()->GetTimerManager().SetTimer(TeleportDelayTimerHandle, this, &UUltSkillNumber_7::Activate_Skill, TeleportDelay, false);
}

void UUltSkillNumber_7::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return; 
    GetWorld()->GetTimerManager().ClearTimer(TeleportDelayTimerHandle);

    OwnerCharacter->SetActorHiddenInGame(true);
    
    // To Do : Teleport 이펙트
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OwnerCharacter->GetAPSkillHubComponent()->GetArcaneTeleportEffect(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

    GetWorld()->GetTimerManager().SetTimer(TeleportDelayTimerHandle, this, &UUltSkillNumber_7::SkillEnd, TeleportDelay_2, false);
}

void UUltSkillNumber_7::SkillEnd()
{
    if(!OwnerCharacter.IsValid()) return;

    TeleportLocation.Z = OwnerCharacter->GetActorLocation().Z;
    OwnerCharacter->SetActorLocation(TeleportLocation);

    OwnerCharacter->SetActorHiddenInGame(false);
    PlayerCollisionChange(true);

    // To Do : Teleport 이펙트
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OwnerCharacter->GetAPSkillHubComponent()->GetArcaneTeleportEffect(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
    OwnerCharacter->GetAttackComponent()->MultiAttack(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, UltSkill7_TargetRange, DamageCoefficient, 1, 0.0f);
    
    OwnerCharacter->SetDoing(false);
    Remove_Skill();

    GetWorld()->GetTimerManager().SetTimer(TeleportDelayTimerHandle, this, &UUltSkillNumber_7::CameraLagStart, TeleportDelay_2, false);
}

void UUltSkillNumber_7::CameraLagStart()
{
    OwnerCharacter->GetAPSpringArm()->bEnableCameraLag = true;
}


void UUltSkillNumber_7::PlayerCollisionChange(bool NewBool)
{
    if(NewBool) // 콜리전 원상태
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else // 콜리전 무시
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void UUltSkillNumber_7::UpdateSkillData()
{

}
