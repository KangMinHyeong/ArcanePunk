// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Volume/APBlockingVolume.h"

// Sets default values
AAPBlockingVolume::AAPBlockingVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

