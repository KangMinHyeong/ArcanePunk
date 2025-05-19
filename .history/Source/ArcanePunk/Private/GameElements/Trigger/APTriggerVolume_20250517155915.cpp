// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Trigger/APTriggerVolume.h"

// Sets default values
AAPTriggerVolume::AAPTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAPTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

