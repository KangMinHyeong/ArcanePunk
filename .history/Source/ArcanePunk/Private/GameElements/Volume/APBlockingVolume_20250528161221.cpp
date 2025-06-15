// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Volume/APBlockingVolume.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

