#include "Components/Character/APTakeDamageComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "Components/CapsuleComponent.h"

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

	DamageApplied = FMath::Min(PD.HP, DamageApplied);

	PD.HP = PD.HP - (DamageApplied * Defense_constant * (1/(Defense_constant + PD.DEF)));
	OwnerCharacter->SetPlayerStatus(PD);
	
	if(OwnerCharacter->IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;
		
		OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//DetachFromControllerPendingDestroy();
	// 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

	}
	else
	{ 
		OwnerCharacter->SetHitting(true);
		if(OwnerCharacter->GetHitMaterial()) OwnerCharacter->GetMesh()->SetMaterial(0, OwnerCharacter->GetHitMaterial());
		AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
		if(MyController) MyController->HitUI();
		UE_LOG(LogTemp, Display, TEXT("Character HP : %f"), OwnerCharacter->GetPlayerStatus().HP);
		GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, this, &UAPTakeDamageComponent::OnHitting, OwnerCharacter->GetHitMotionTime(), false);
	}
}

void UAPTakeDamageComponent::OnHitting()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetHitting(true);
	OwnerCharacter->GetMesh()->SetMaterial(0,OwnerCharacter->GetDefaultMaterial());
	GetWorld()->GetTimerManager().ClearTimer(HitTimerHandle);
}
