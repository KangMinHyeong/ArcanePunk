// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FootPrint/APFootPrint.h"

#include "Components/DecalComponent.h"

// Sets default values
AAPFootPrint::AAPFootPrint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	FootDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("FootDecal"));

	SetRootComponent(RootComp);
	FootDecal->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AAPFootPrint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPFootPrint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

