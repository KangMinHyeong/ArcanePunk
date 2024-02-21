#include "Enemy/APDronEnemyCharacter.h"
#include <Enemy/APDron.h>
#include "Engine/World.h"
#include "TimerManager.h"

AAPDronEnemyCharacter::AAPDronEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AAPDronEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AAPDronEnemyCharacter::SpawnDron, 5.0f, true, 0.0f);

}

void AAPDronEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AAPDronEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAPDronEnemyCharacter::SpawnDron()
{
    if (DronCount < MaxDronCount)
    {
        FString BlueprintPath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_APDron.BP_APDron_C'");
        UClass* BPClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *BlueprintPath));
        FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 300);

        AAPDron* NewDron = GetWorld()->SpawnActor<AAPDron>(BPClass, SpawnLocation, FRotator::ZeroRotator);

        if (NewDron)
        {
            DronCount++;
        }
    }
}

void AAPDronEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

