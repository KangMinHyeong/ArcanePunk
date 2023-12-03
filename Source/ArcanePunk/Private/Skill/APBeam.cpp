// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/APBeam.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AAPBeam::AAPBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BeamRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamRoot"));
	RootComponent = BeamRoot;

}

// Called when the game starts or when spawned
void AAPBeam::BeginPlay()
{
	Super::BeginPlay();
	
	if(!BeamEffect) return;
	BeamEffectComp = UGameplayStatics::SpawnEmitterAttached(BeamEffect, BeamRoot, TEXT("BeamEffect"), BeamRoot->GetComponentLocation(), BeamRoot->GetComponentRotation());
}

// Called every frame
void AAPBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartBeam();
}

void AAPBeam::StartBeam()
{
	FVector Start = BeamRoot->GetComponentLocation();
	FVector End = Start + BeamRoot->GetForwardVector() * BeamDistance;

	if(!BeamEffectComp) return;
	BeamEffectComp->SetBeamSourcePoint(0, Start, 0);
	BeamEffectComp->SetBeamEndPoint(0, End);
}
