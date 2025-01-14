// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Install/APTurret.h"

AAPTurret::AAPTurret()
{
    RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
    
    RotateMesh->SetupAttachment(InstallRoot);
}

void AAPTurret::BeginPlay()
{
    Super::BeginPlay();

    OriginAmmoNum = SpawnAmmoNum;
    Enemy = FindEnemy();

    SetActorTickEnabled(true);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AArcaneTurret::Fire, FireRate, true);
}

void AAPTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAPTurret::DestroySKill()
{
}

