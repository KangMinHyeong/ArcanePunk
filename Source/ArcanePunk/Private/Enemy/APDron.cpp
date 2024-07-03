#include "Enemy/APDron.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../../Public/Enemy/APDron.h"
#include "../../Public/Character/ArcanePunkCharacter.h"

AAPDron::AAPDron()
{
    PrimaryActorTick.bCanEverTick = true;

    NeighborRadius = 1000.0f;
    AlignmentWeight = 1.0f;
    CohesionWeight = 1.0f;
    SeparationWeight = 1.0f;
    MoveSpeed = 30.0f;
    SeparationScale = 1.0f;
}

void AAPDron::BeginPlay()
{
    Super::BeginPlay();

    SetGroupID();
    InitialLocation = GetActorLocation();
}

void AAPDron::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

        FVector SeparationForce = CalculateSeparation() * SeparationWeight;
        FVector CohesionForce = CalculateCohesion() * CohesionWeight;
        FVector TotalForce = SetDronDirection() + SeparationForce + CohesionForce;

        FVector NewDirection = TotalForce.GetSafeNormal();
        FVector NewLocation = GetActorLocation() + NewDirection * MoveSpeed * DeltaTime;

        SetActorLocation(NewLocation);
}

void AAPDron::SetGroupID()
{
    GroupID = FMath::RandRange(1, 4);
}

FVector AAPDron::CalculateCohesion()
{
    FVector CenterOfMass = FVector::ZeroVector;

    TArray<AActor*> Neighbors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAPDron::StaticClass(), Neighbors);

    int32 NeighborCount = 0;
    for (AActor* Neighbor : Neighbors)
    {
        AAPDron* FlockingNeighbor = Cast<AAPDron>(Neighbor);
        if (FlockingNeighbor && FlockingNeighbor->GroupID == GroupID && Neighbor != this &&
            FVector::Dist(GetActorLocation(), Neighbor->GetActorLocation()) < NeighborRadius)
        {
            CenterOfMass += FlockingNeighbor->GetActorLocation();
            NeighborCount++;
            FlockingNeighbor->IsClustered = true;
        }
    }

    if (NeighborCount > 0)
    {
        CenterOfMass /= NeighborCount;
        return (CenterOfMass - GetActorLocation()).GetSafeNormal();
    }

    return FVector::ZeroVector;
}

FVector AAPDron::CalculateSeparation()
{
    FVector SeparationForce = FVector::ZeroVector;

    TArray<AActor*> Neighbors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAPDron::StaticClass(), Neighbors);

    for (AActor* Neighbor : Neighbors)
    {
        AAPDron* FlockingNeighbor = Cast<AAPDron>(Neighbor);
        if (FlockingNeighbor && FlockingNeighbor->GroupID == GroupID && Neighbor != this &&
            FVector::Dist(GetActorLocation(), Neighbor->GetActorLocation()) < NeighborRadius)
        {
            FVector ToNeighbor = GetActorLocation() - Neighbor->GetActorLocation();
            float Distance = ToNeighbor.Size();

            if (Distance > 0)
            {
                float SeparationScaleModified = SeparationScale;
                if (Distance < 150)
                {
                    SeparationScaleModified *= 2.0f;
                }

                SeparationForce += ToNeighbor.GetSafeNormal() * (SeparationScaleModified / Distance);
            }
        }
    }

    return SeparationForce.GetSafeNormal();
}

FVector AAPDron::SetDronDirection()
{
    ACharacter* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (PlayerActor)
    {
        FVector ToPlayer = PlayerActor->GetActorLocation() - GetActorLocation();
        ToPlayer.Z = 0.0f;

        if (FMath::Abs(ToPlayer.X) <= 100.0f && FMath::Abs(ToPlayer.Y) <= 100.0f)
        {
            return FVector::ZeroVector;
            IsClustered = true;
        }

        FVector NewDirection = ToPlayer.GetSafeNormal();
        return NewDirection;
    }

    return FVector::ZeroVector;
}