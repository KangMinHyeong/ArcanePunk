// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/APEnemyDronCharacter.h"
#include "Enemy/APDron.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

AAPEnemyDronCharacter::AAPEnemyDronCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAPEnemyDronCharacter::BeginPlay()
{
	Super::BeginPlay();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AAPEnemyDronCharacter::SpawnDron, 5.0f, true, 0.0f);
}

// Called every frame
void AAPEnemyDronCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAPEnemyDronCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AAPEnemyDronCharacter::SpawnDron()
{
    if (DronCount < MaxDronCount)
    {
        FString BlueprintPath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BPAPDron.BPAPDron_C'");
        UClass* DronClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *BlueprintPath));

        FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 300);

        AAPDron* NewDron = GetWorld()->SpawnActor<AAPDron>(DronClass, SpawnLocation, FRotator::ZeroRotator);
        DronCount++;
    }
}

void AAPEnemyDronCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}