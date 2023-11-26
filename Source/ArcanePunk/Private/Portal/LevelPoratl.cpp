// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/LevelPoratl.h"
#include "Kismet/GameplayStatics.h"

ALevelPoratl::ALevelPoratl()
{
}

void ALevelPoratl::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelPoratl::OnTeleport_A(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
}

