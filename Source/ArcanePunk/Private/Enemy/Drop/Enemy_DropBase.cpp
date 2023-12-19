// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropBase.h"

#include "Components/SphereComponent.h"

// Sets default values
AEnemy_DropBase::AEnemy_DropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DropTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DropTrigger"));
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));

	SetRootComponent(DropTrigger);
	DropMesh->SetupAttachment(DropTrigger);
}

// Called when the game starts or when spawned
void AEnemy_DropBase::BeginPlay()
{
	Super::BeginPlay();
	
	DropTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_DropBase::DropOverlap);
}

// Called every frame
void AEnemy_DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_DropBase::DropOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
}
