
#include "GameElements/Drop/APManaEnergy.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"

AAPManaEnergy::AAPManaEnergy()
{
 	PrimaryActorTick.bCanEverTick = false;

	EnergyTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("EnergyTrigger"));
	EnergyEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EnergyEffect"));
	EnergyMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("EnergyMoveComp"));

	SetRootComponent(EnergyTrigger);
	EnergyEffect->SetupAttachment(EnergyTrigger);
}

void AAPManaEnergy::BeginPlay()
{
	Super::BeginPlay();
	
	EnergyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPManaEnergy::OnOverlap);

}

void AAPManaEnergy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPManaEnergy::SetEnergyMoveComp(AActor* ManaOwner)
{
	EnergyMoveComp->bIsHomingProjectile = true;
    EnergyMoveComp->HomingAccelerationMagnitude = HomingSpeed;
	EnergyMoveComp->HomingTargetComponent = ManaOwner->GetRootComponent();
}

void AAPManaEnergy::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	// Mana 회복
}
