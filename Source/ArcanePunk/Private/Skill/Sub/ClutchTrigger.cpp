
#include "Skill/Sub/ClutchTrigger.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APHitPointComponent.h"
#include "Character/ArcanePunkCharacter.h"

AClutchTrigger::AClutchTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	ClutchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ClutchSphere"));
	ClutchEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ClutchEffect"));

	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));

	SetRootComponent(ClutchSphere);
	ClutchEffect->SetupAttachment(ClutchSphere);
}

void AClutchTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	ClutchSphere->OnComponentBeginOverlap.AddDynamic(this, &AClutchTrigger::OnOverlap);
}

void AClutchTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClutchTrigger::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	auto Character = Cast<AArcanePunkCharacter>(MyOwner);
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && Character)
	{
		HitPointComp->DistinctHitPoint(SweepResult.Location, OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, Character->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
        // if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
	}
}

void AClutchTrigger::SetClutchSphere(float NewValue)
{
	SetActorScale3D(GetActorScale3D() * NewValue / ClutchSphere->GetScaledSphereRadius());
}

void AClutchTrigger::SetDestroy(float Time)
{
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AClutchTrigger::OnDestroy, Time, false);
}

void AClutchTrigger::OnDestroy()
{
	Destroy();
}
