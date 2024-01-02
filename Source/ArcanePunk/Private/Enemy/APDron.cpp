#include "Enemy/APDron.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

AAPDron::AAPDron()
{
	PrimaryActorTick.bCanEverTick = true;

    MovementSpeed = 1.0f;
    InfluenceRadius = 500.0f;
}

void AAPDron::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPDron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<AActor*> NearbyDrons;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAPDron::StaticClass(), NearbyDrons);

    FVector AveragePosition = FVector::ZeroVector;
    int32 NearbyDronCount = 0;

    for (AActor* NearbyDron : NearbyDrons)
    {
        if (NearbyDron != this)
        {
            float Distance = FVector::Distance(GetActorLocation(), NearbyDron->GetActorLocation());

            if (Distance < InfluenceRadius)
            {
                AveragePosition += NearbyDron->GetActorLocation();
                NearbyDronCount++;
            }
        }
    }

    if (NearbyDronCount > 0)
    {
        AveragePosition /= NearbyDronCount;

        FVector RandomDisplacement = FVector(FMath::RandRange(-150.0f, 150.0f), FMath::RandRange(-150.0f, 150.0f), 0.0f);
        AveragePosition += RandomDisplacement;

        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), AveragePosition, DeltaTime, MovementSpeed);
        SetActorLocation(NewLocation);
    }
}

