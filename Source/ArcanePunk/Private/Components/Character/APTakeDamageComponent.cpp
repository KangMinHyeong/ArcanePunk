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
#include "Components/Character/APPassiveComponent.h"

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

void UAPTakeDamageComponent::InitTakeDamageComp()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
}

void UAPTakeDamageComponent::DamageCalculation(float &DamageApplied)
{
	bool Check = false;
	if(OwnerCharacter->GetAPPassiveComp()->IsDamagedPassive()) Check = CheckingDamaged();
	if(Check) return;

	auto PD = OwnerCharacter->GetPlayerStatus();

	DamageApplied = FMath::Min(PD.StatusData.HP, DamageApplied);

	if(DamageApplied >= PD.StatusData.HP && OwnerCharacter->GetRageMode()) { PD.StatusData.HP = 1.0f;}
	else
	{
		PD.StatusData.HP = PD.StatusData.HP - (DamageApplied * Defense_constant * (1/(Defense_constant + PD.StatusData.DEF)));
	}
	OwnerCharacter->SetDefaultHP(PD.StatusData.HP); 
	OwnerCharacter->SetDefaultHP_Origin(PD.StatusData.HP); 

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
		UE_LOG(LogTemp, Display, TEXT("Character HP : %f"), OwnerCharacter->GetPlayerStatus().StatusData.HP);
	}
}

void UAPTakeDamageComponent::ReflectDamage(float & DamageApplied, AActor* DamageCauser)
{
	if(!OwnerCharacter.IsValid()) return;

	FHitResult Hit;
	Hit.Location = OwnerCharacter->GetActorLocation(); Hit.ImpactPoint = Hit.Location;
	UGameplayStatics::ApplyPointDamage(DamageCauser, DamageApplied, Hit.ImpactPoint, Hit, OwnerCharacter->GetInstigatorController(), OwnerCharacter.Get(), UDamageType::StaticClass());
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

bool UAPTakeDamageComponent::CheckingDamaged()
{
	if(!OwnerCharacter.IsValid()) return false;
	DamagedNumber++;
	UE_LOG(LogTemp, Display, TEXT("DamagedNumber %d"), DamagedNumber);
	if(DamagedNumber == 8 && DamagedShield.IsValid()) {DamagedNumber = 0; DamagedShield->Destroy(); return true;}
	else if(DamagedNumber == 7)
	{
		DamagedShield = GetWorld()->SpawnActor<AActor>(DamagedShieldClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		DamagedShield->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	}

    return false;
}

void UAPTakeDamageComponent::SetHitEffect(FVector HitLocation)
{
	if(!OwnerCharacter.IsValid()) return;

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

