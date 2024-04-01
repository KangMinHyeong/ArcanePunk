#include "Components/Character/APTakeDamageComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Kismet/GameplayStatics.h"

UAPTakeDamageComponent::UAPTakeDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPTakeDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAPTakeDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPTakeDamageComponent::DamageCalculation(float &DamageApplied)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto PD = OwnerCharacter->GetPlayerStatus();

	DamageApplied = FMath::Min(PD.PlayerDynamicData.HP, DamageApplied);

	GetWorld()->GetTimerManager().SetTimer(HittingTimerHandle, this, &UAPTakeDamageComponent::OnHitting, HitMotionTime, false);

	if(DamageApplied == PD.PlayerDynamicData.HP && OwnerCharacter->GetRageMode()) { PD.PlayerDynamicData.HP = 1.0f;}
	else
	{
		PD.PlayerDynamicData.HP = PD.PlayerDynamicData.HP - (DamageApplied * Defense_constant * (1/(Defense_constant + PD.PlayerDynamicData.DEF)));
	}
	OwnerCharacter->SetPlayerStatus(PD);
	
	if(OwnerCharacter->IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;
		OwnerCharacter->SetCanMove(false);
		OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// OwnerCharacter->DetachFromControllerPendingDestroy();
		OwnerCharacter->DeadPenalty(DeathTime);
		auto GM = Cast<AAPGameModeBattleStage>((UGameplayStatics::GetGameMode(GetWorld()))); if(!GM) return;
		GM->PlayerKilled();
	// 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

	}
	else
	{ 
		if(OwnerCharacter->GetRageMode()) return;
		OwnerCharacter->SetHitting(true);
		AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
		if(MyController) MyController->HitUI();
		UE_LOG(LogTemp, Display, TEXT("Character HP : %f"), OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HP);
	}
}

void UAPTakeDamageComponent::SetHitPoint(float Forward, float Right)
{
	PlayerIsForward = Forward;
	PlayerIsRight = Right;
}

float UAPTakeDamageComponent::GetForward()
{
    return PlayerIsForward;
}

float UAPTakeDamageComponent::GetRight()
{
    return PlayerIsRight;
}

void UAPTakeDamageComponent::OnHitting()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetHitting(false);
	if(OwnerCharacter->GetHideMode()) { OwnerCharacter->GetMesh()->SetMaterial(0,OwnerCharacter->GetHideMaterial()); }
	else {OwnerCharacter->GetMesh()->SetMaterial(0,OwnerCharacter->GetDefaultMaterial());}
	
	GetWorld()->GetTimerManager().ClearTimer(HittingTimerHandle);
}

void UAPTakeDamageComponent::SetHitEffect(FVector HitLocation)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(HitMaterial) OwnerCharacter->GetMesh()->SetMaterial(0, HitMaterial);

	if(PlayerIsForward > 0)
	{
		if(PlayerIsRight > 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_R, OwnerCharacter->GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_L, OwnerCharacter->GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
		}
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_B, OwnerCharacter->GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
	}
}

